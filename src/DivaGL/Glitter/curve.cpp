/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    CurveX::Key::Key() : type(), frame(), value(), tangent1(), tangent2(), random_range() {

    }

    CurveX::Key::Key(KeyType type, int32_t frame,
        float_t value, float_t random_range) : tangent1(), tangent2() {
        this->type = type;
        this->frame = frame;
        this->value = value;
        this->random_range = random_range;
    }

    CurveX::Key::Key(KeyType type, int32_t frame, float_t value,
        float_t tangent1, float_t tangent2, float_t random_range) {
        this->type = type;
        this->frame = frame;
        this->value = value;
        this->tangent1 = tangent1;
        this->tangent2 = tangent2;
        this->random_range = random_range;
    }

    CurveX::CurveX() : type(), repeat(),
        start_time(), end_time(), flags(), random_range() {
        version = 0x02;
        keys_version = 0x03;
        keys.reserve(0x80);
    }

    CurveX::~CurveX() {

    }

    bool CurveX::GetValue(float_t frame,
        float_t* value, int32_t random_value, RandomX* random) {
        size_t keys_count = keys.size();
        if (!keys_count)
            return false;

        int32_t random_val = random->GetValue();
        random->SetValue(random_value);
        bool negate = flags & CURVE_NEGATE && random->GetInt(0, 0xFFFF) > 0x7FFF;
        if (flags & CURVE_STEP)
            random->SetValue(random_val + 1);

        float_t start_time;
        float_t end_time;
        float_t _value;
        if (keys_count == 1) {
            const CurveX::Key& key = keys.data()[0];
            _value = RandomizeKey(key, random);
            goto End;
        }

        start_time = (float_t)this->start_time;
        end_time = (float_t)this->end_time;
        if (repeat && (start_time > frame || frame >= end_time)) {
            float_t t = (frame - start_time) / (end_time - start_time);
            if (t > 0.0f)
                t = (float_t)(int32_t)t;
            else if (t < 0.0f)
                t = (float_t)(int32_t)t - 1.0f;
            frame -= t * (end_time - start_time);
        }

        if (end_time <= frame) {
            const CurveX::Key& key = keys.data()[keys_count - 1];
            _value = RandomizeKey(key, random);
        }
        else if (start_time > frame) {
            const CurveX::Key& key = keys.data()[0];
            _value = RandomizeKey(key, random);
        }
        else if (flags & CURVE_BAKED) {
            size_t key_index;
            if (frame >= end_time)
                key_index = keys_count - 1;
            else if (frame > start_time) {
                key_index = (size_t)frame - this->start_time;
                if (!(flags & CURVE_BAKED_FULL))
                    key_index /= 2;

                if (key_index >= keys_count)
                    key_index = keys_count - 1;
            }
            else
                key_index = 0;

            const CurveX::Key& key = keys.data()[key_index];
            _value = RandomizeKey(key, random);
        }
        else {
            size_t curr_key_index = 0;
            size_t next_key_index = 0;
            if (keys_count > 3)
                CurveX::GetKeyIndices(keys, frame, curr_key_index, next_key_index);
            else if (keys_count == 3 && frame >= keys.data()[1].frame) {
                curr_key_index = 1;
                next_key_index = 2;
            }
            else {
                curr_key_index = 0;
                next_key_index = 1;
            }
            const CurveX::Key& curr_key = keys.data()[curr_key_index];
            const CurveX::Key& next_key = keys.data()[next_key_index];
            _value = Interpolate(frame, curr_key, next_key, curr_key.type, random);
        }

    End:
        _value = Randomize(_value, random);
        *value = negate ? -_value : _value;
        random->SetValue(random_val + 1);
        return true;
    }

    float_t CurveX::Interpolate(float_t frame, const CurveX::Key& curr,
        const CurveX::Key& next, KeyType key_type, RandomX* random) {
        if (key_type == KEY_CONSTANT)
            return RandomizeKey(curr, random);
        else if (key_type == KEY_HERMITE)
            return InterpolateHermite(curr, next, frame, random);
        else
            return InterpolateLinear(curr, next, frame, random);
    }

    float_t CurveX::InterpolateHermite(const CurveX::Key& curr,
        const CurveX::Key& next, float_t frame, RandomX* random) {
        float_t next_val = RandomizeKey(next, random);
        float_t curr_val = RandomizeKey(curr, random);
        return InterpolateHermite(RandomizeKey(curr, random),
            next_val - curr_val, curr.tangent2, next.tangent1,
            (float_t)curr.frame, (float_t)next.frame, frame);
    }

    float_t CurveX::InterpolateLinear(const CurveX::Key& curr,
        const CurveX::Key& next, float_t frame, RandomX* random) {
        float_t df = (float_t)(next.frame - curr.frame);
        float_t t = (frame - (float_t)curr.frame) / (float_t)(next.frame - curr.frame);
        float_t curr_val = RandomizeKey(curr, random);
        float_t next_val = RandomizeKey(next, random);
        return curr_val * (1.0f - t) + next_val * t;
    }

    float_t CurveX::Randomize(float_t value, RandomX* random) {
        if (!(flags & CURVE_RANDOM_RANGE))
            return value;

        float_t rand = random->GetFloat(flags & CURVE_RANDOM_RANGE_NEGATE
            ? -random_range : 0.0f, random_range);

        if (flags & CURVE_RANDOM_RANGE_MULT) {
            rand *= 0.01f;
            rand *= value;
        }
        return value + rand;
    }

    float_t CurveX::RandomizeKey(const CurveX::Key& key, RandomX* random) {
        if (!(flags & CURVE_KEY_RANDOM_RANGE))
            return key.value;
        else if (flags & CURVE_BAKED)
            return random->GetFloat(key.min_value, key.max_value);

        return key.value + random->GetFloat(flags & CURVE_RANDOM_RANGE_NEGATE
            ? -key.random_range : 0.0f, key.random_range);
    }

    void CurveX::GetKeyIndices(const prj::vector<CurveX::Key>& keys,
        float_t frame, size_t& curr, size_t& next) {
        size_t count = keys.size();
        if (count <= 1) {
            curr = 0;
            next = 0;
            return;
        }

        size_t first_key = 0;
        const CurveX::Key* key = keys.data();
        size_t last_key = count - 1;
        size_t temp = last_key / 2;
        while (first_key <= last_key) {
            temp = (last_key + first_key) / 2;
            if (frame <= key[(temp + 1) % count].frame) {
                if (frame >= key[temp].frame)
                    goto NextKey;
                last_key = temp - 1;
            }
            else
                first_key = temp + 1;
        }

        if (frame > key[temp].frame) {
        NextKey:
            curr = temp;
            next = temp + 1;
        }
        else {
            curr = temp - 1;
            next = temp;
        }
        next %= count;
    }
}

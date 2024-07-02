/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "rob_osage_test.hpp"
#include "../../KKdLib/io/file_stream.hpp"
#include "../../KKdLib/io/path.hpp"
#include "../../KKdLib/prj/algorithm.hpp"
#include "../../KKdLib/key_val.hpp"
#include "../Glitter/glitter.hpp"
#include "../rob/rob.hpp"
#include "../bone_database.hpp"
#include "../canonical_properties.hpp"
#include "../dw.hpp"
#include "../render_context.hpp"
#include "../resolution_mode.hpp"
#include <Helpers.h>

class RobOsageTestDw : public dw::Shell {
public:
    struct Rob {
        dw::ListBox* list_box;

        dw::Button* save_button;
        dw::Button* load_button;
        dw::Button* default_button;

        dw::ListBox* object_list_box;

        void Init(dw::Composite* parent);

        static void CharaCallback(dw::Widget* data);
        static void DefaultCallback(dw::Widget* data);
        static void LoadCallback(dw::Widget* data);
        static void ObjectCallback(dw::Widget* data);
        static void SaveCallback(dw::Widget* data);
    };

    struct Flags {
        dw::Button* line_button;
        dw::Button* lock_button;
        dw::Button* no_pause_button;
        dw::Button* coli_button;
        dw::Button* name_button;
        dw::Button* init_button;

        void Init(dw::Composite* parent);

        static void ColiCallback(dw::Widget* data);
        static void InitCallback(dw::Widget* data);
        static void LineCallback(dw::Widget* data);
        static void LockCallback(dw::Widget* data);
        static void NameCallback(dw::Widget* data);
        static void NoPauseCallback(dw::Widget* data);
    };

    struct Root {
        class Force : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            Force(dw::Composite* parent);
            virtual ~Force() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        class Gain : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            Gain(dw::Composite* parent);
            virtual ~Gain() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        class AirRes : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            AirRes(dw::Composite* parent);
            virtual ~AirRes() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        class RootYRot : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            RootYRot(dw::Composite* parent);
            virtual ~RootYRot() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        class RootZRot : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            RootZRot(dw::Composite* parent);
            virtual ~RootZRot() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        class Fric : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            Fric(dw::Composite* parent);
            virtual ~Fric() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        class WindAfc : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            WindAfc(dw::Composite* parent);
            virtual ~WindAfc() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        class ColiType : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::ListBox* list_box;

            ColiType(dw::Composite* parent);
            virtual ~ColiType() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetItemIndex(size_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        class InitYRot : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            InitYRot(dw::Composite* parent);
            virtual ~InitYRot() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        class InitZRot : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            InitZRot(dw::Composite* parent);
            virtual ~InitZRot() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        dw::ListBox* list_box;

        dw::Button* reset_button;
        dw::Button* copy_button;
        dw::Button* paste_button;

        Force* force;
        Gain* gain;
        AirRes* air_res;
        RootYRot* root_y_rot;
        RootZRot* root_z_rot;
        Fric* fric;
        WindAfc* wind_afc;
        ColiType* coli_type;
        InitYRot* init_y_rot;
        InitZRot* init_z_rot;

        void Init(dw::Composite* parent);
        void Update();

        static void CopyCallback(dw::Widget* data);
        static void OsageCallback(dw::Widget* data);
        static void PasteCallback(dw::Widget* data);
        static void ResetCallback(dw::Widget* data);
    };

    struct Node {
        class CollisionRadius : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            CollisionRadius(dw::Composite* parent);
            virtual ~CollisionRadius() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        class Hinge : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* y_min_slider;
            dw::Slider* y_max_slider;
            dw::Slider* z_min_slider;
            dw::Slider* z_max_slider;

            Hinge(dw::Composite* parent);
            virtual ~Hinge() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(const RobOsageTest::Node::Hinge& value);
            void Update(bool value);

            static void YMinCallback(dw::Widget* data);
            static void YMaxCallback(dw::Widget* data);
            static void ZMinCallback(dw::Widget* data);
            static void ZMaxCallback(dw::Widget* data);
        };

        class InertialCancel : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            InertialCancel(dw::Composite* parent);
            virtual ~InertialCancel() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        class Weight : public dw::SelectionAdapter {
        public:
            dw::Composite* comp;
            dw::Button* button;
            dw::Slider* slider;

            Weight(dw::Composite* parent);
            virtual ~Weight() override;

            virtual void Callback(dw::SelectionListener::CallbackData* data) override;

            void SetValue(float_t value);
            void Update(bool value);

            static void Callback(dw::Widget* data);
        };

        dw::Group* group;

        CollisionRadius* coli_r;
        Hinge* hinge;
        InertialCancel* inertial_cancel;
        Weight* weight;

        void Init(dw::Composite* parent);
        void Update();
    };

    struct BetweenOsageCollision {
        dw::Button* button;

        void Init(dw::Composite* parent);
    };

    struct Collision {
        dw::Group* group;

        dw::Button* reset_button;
        dw::Button* copy_button;
        dw::Button* paste_button;

        dw::Composite* element_comp;

        void Init(dw::Composite* parent);
        void Update();

        static void CopyCallback(dw::Widget* data);
        static void ElementCallback(dw::Widget* data);
        static void PasteCallback(dw::Widget* data);
        static void ResetCallback(dw::Widget* data);
    };

    struct ColliElement {
        dw::Group* group;

        dw::Button* reset_button;
        dw::Button* copy_button;
        dw::Button* paste_button;
        dw::Button* flip_button;

        dw::ListBox* type_list_box;
        dw::Slider* radius_slider;
        dw::ListBox* bone0_list_box;
        dw::Slider* bone0_x_slider;
        dw::Slider* bone0_y_slider;
        dw::Slider* bone0_z_slider;
        dw::ListBox* bone1_list_box;
        dw::Slider* bone1_x_slider;
        dw::Slider* bone1_y_slider;
        dw::Slider* bone1_z_slider;

        void Init(dw::Composite* parent);
        void Update();

        static void BoneCallback(dw::Widget* data);
        static void BonePosXCallback(dw::Widget* data);
        static void BonePosYCallback(dw::Widget* data);
        static void BonePosZCallback(dw::Widget* data);
        static void CopyCallback(dw::Widget* data);
        static void FlipCallback(dw::Widget* data);
        static void PasteCallback(dw::Widget* data);
        static void RadiusCallback(dw::Widget* data);
        static void ResetCallback(dw::Widget* data);
        static void TypeCallback(dw::Widget* data);
    };

    Rob rob;
    Flags flags;
    Root root;
    Node node;
    BetweenOsageCollision boc;
    Collision collision;
    ColliElement colli_element;

    RobOsageTestDw();
    virtual ~RobOsageTestDw() override;

    virtual void Hide() override;
};

const char* collision_type_name_list[] = {
    "END",
    "BALL",
    "CYLINDER",
    "PLANE",
    "ELLIPSE",
    "AABB",
};

RobOsageTest* rob_osage_test;
RobOsageTestDw* rob_osage_test_dw;

extern render_context* rctx_ptr;

RobOsageTest::RobOsageTest() : load(), save(), coli(), line(),
osage_index(), collision_index(), collision_update() {
    chara_id = -1;
    load_chara_id = 0;
    item_id = ITEM_NONE;
    osage_index = -1;
    collision_index = -1;
}

RobOsageTest::~RobOsageTest() {

}

bool RobOsageTest::init() {
    if (!rob_osage_test_dw) {
        rob_osage_test_dw = new RobOsageTestDw;
        rob_osage_test_dw->sub_1402F38B0();
    }
    else
        rob_osage_test_dw->Disp();
    return true;
}

bool RobOsageTest::ctrl() {
    if (load) {
        load = false;

        chara_id = load_chara_id;

        objects.clear();
        bocs.clear();
        normal_refs.clear();

        size_t object_index = rob_osage_test_dw->rob.object_list_box->list->selected_item;
        rob_osage_test_dw->rob.object_list_box->ClearItems();
        rob_osage_test_dw->rob.object_list_box->SetItemIndex(-1);

        rob_chara_item_equip* rob_itm_equip = rob_chara_array_get_item_equip(get_rob_chara_smth(), chara_id);
        if (!rob_itm_equip)
            return false;

        for (int32_t i = 0; i < ITEM_MAX; i++) {
            rob_chara_item_equip_object* itm_eq_obj = rob_itm_equip->get_item_equip_object((::item_id)i);
            ::obj* obj = objset_info_storage_get_object(itm_eq_obj->obj_info);
            if (!obj || !itm_eq_obj->osage_blocks.size() && !itm_eq_obj->cloth_blocks.size())
                continue;

            objects.push_back((::item_id)i, itm_eq_obj->obj_info);
            rob_osage_test_dw->rob.object_list_box->AddItem(object_database_get_object_name(itm_eq_obj->obj_info));

            std::string buf = string_to_lower(sprintf_s_string(
                "ext_skp_%s.txt", object_database_get_object_name(itm_eq_obj->obj_info)));

            std::string path("ram/skin_param/");
            path.append(buf);

            key_val kv;
            {
                CanonicalProperties can_prop;
                if (path_check_file_exists(path.c_str())) {
                    file_stream s;
                    s.open(path.c_str(), "rb");
                    if (s.check_not_null()) {
                        char* d = force_malloc<char>(s.length);
                        s.read(d, s.length);
                        can_prop.ParseEscapeSeq((const char*)d, s.length);
                        free_def(d);
                    }
                }
                else {
                    path.assign("rom/skin_param/");
                    path.append(buf);

                    p_file_handler pfhndl;
                    pfhndl.read_file_path(path.c_str(), prj::HeapCMallocDebug);
                    pfhndl.read_now();
                    if (!pfhndl.check_not_ready())
                        can_prop.ParseEscapeSeq((const char*)pfhndl.get_data(), pfhndl.get_size());
                }
                itm_eq_obj->skp_load(&can_prop);

                kv.parse(can_prop.data.data(), can_prop.data.size());
            }

            prj::vector_pair_alloc<prj::string, ExNodeBlock*> ex_nodes;
            ex_nodes.reserve(itm_eq_obj->osage_blocks.size() + itm_eq_obj->cloth_blocks.size());

            for (ExOsageBlock*& j : itm_eq_obj->osage_blocks) {
                ExOsageBlock* osg = j;
                ex_nodes.push_back({ osg->name, osg });
            }

            for (ExClothBlock*& j : itm_eq_obj->cloth_blocks) {
                ExClothBlock* cls = j;
                ex_nodes.push_back({ cls->name, cls });
            }

            ex_nodes.sort();

            for (ExOsageBlock*& j : itm_eq_obj->osage_blocks) {
                ExOsageBlock* osg = j;
                if (!kv.open_scope(osg->name))
                    continue;

                if (!kv.open_scope("node")) {
                    kv.close_scope();
                    continue;
                }

                int32_t node_length = 0;
                kv.read("length", node_length);

                kv.close_scope();

                if (!kv.open_scope("root")) {
                    kv.close_scope();
                    continue;
                }

                int32_t count;
                if (kv.read("boc", "length", count) && count > 0) {
                    bocs.push_back(osg, {});
                    prj::vector<skin_param_osage_root_boc>& vb = bocs.back().second;

                    vb.reserve(count);
                    for (int32_t k = 0; k < count; k++) {
                        if (!kv.open_scope_fmt(k))
                            continue;

                        int32_t st_node = 0;
                        std::string ed_root;
                        int32_t ed_node = 0;
                        if (kv.read("st_node", st_node)
                            && st_node < node_length
                            && kv.read("ed_root", ed_root)
                            && kv.read("ed_node", ed_node)
                            && ed_node < node_length) {
                            vb.push_back({});
                            skin_param_osage_root_boc& b = vb.back();
                            b.st_node = st_node;
                            b.ed_root.assign(ed_root.data(), ed_root.size());
                            b.ed_node = ed_node;
                        }
                        kv.close_scope();
                    }
                    kv.close_scope();
                }
                kv.close_scope();

                if (kv.read("normal_ref", "length", count)) {
                    normal_refs.push_back(osg, {});
                    prj::vector<skin_param_osage_root_normal_ref>& vnr = normal_refs.back().second;

                    vnr.reserve(count);
                    for (int32_t k = 0; k < count; k++) {
                        if (!kv.open_scope_fmt(k))
                            continue;

                        std::string n;
                        if (kv.read("N", n)) {
                            std::string u;
                            std::string d;
                            std::string l;
                            std::string r;
                            kv.read("U", u);
                            kv.read("D", d);
                            kv.read("L", l);
                            kv.read("R", r);

                            vnr.push_back({});
                            skin_param_osage_root_normal_ref& nr = vnr.back();
                            nr.n.assign(n.data(), n.size());
                            nr.u.assign(u.data(), u.size());
                            nr.d.assign(d.data(), d.size());
                            nr.l.assign(l.data(), l.size());
                            nr.r.assign(r.data(), r.size());
                        }
                        kv.close_scope();
                    }
                    kv.close_scope();
                }

                kv.close_scope();
                kv.close_scope();
            }
        }

        bocs.sort();
        normal_refs.sort();

        bool found = false;
        for (auto& i : objects)
            if (i.first == item_id && i.second == obj_info) {
                found = true;
                break;
            }

        if (!found) {
            rob_osage_test_dw->root.list_box->ClearItems();
            rob_osage_test_dw->root.list_box->SetItemIndex(-1);

            item_id = ITEM_NONE;
            obj_info = object_info();
            osage_index = -1;
            collision_index = -1;
        }
        else
            rob_osage_test_dw->rob.object_list_box->SetItemIndex(object_index);
    }

    if (save) {
        save = false;

        const char* flt_fmt = "%0.6f";

        rob_chara_item_equip* rob_itm_equip = rob_chara_array_get_item_equip(get_rob_chara_smth(), chara_id);
        if (!rob_itm_equip)
            return false;

        bone_database_skeleton* skeleton = bone_database_get_skeleton_by_type(BONE_DATABASE_SKELETON_COMMON);

        for (int32_t i = 0; i < ITEM_MAX; i++) {
            rob_chara_item_equip_object* itm_eq_obj = rob_itm_equip->get_item_equip_object((::item_id)i);
            ::obj* obj = objset_info_storage_get_object(itm_eq_obj->obj_info);
            if (!obj || !itm_eq_obj->osage_blocks.size() && !itm_eq_obj->cloth_blocks.size())
                continue;

            path_create_directory("ram/skin_param/");

            std::string buf = string_to_lower(sprintf_s_string(
                "ext_skp_%s.txt", object_database_get_object_name(itm_eq_obj->obj_info)));

            prj::vector_pair_alloc<prj::string, ExNodeBlock*> ex_nodes;
            ex_nodes.reserve(itm_eq_obj->osage_blocks.size() + itm_eq_obj->cloth_blocks.size());

            for (ExOsageBlock*& i : itm_eq_obj->osage_blocks) {
                ExOsageBlock* osg = i;
                ex_nodes.push_back({ osg->name, osg });
            }

            for (ExClothBlock*& i : itm_eq_obj->cloth_blocks) {
                ExClothBlock* cls = i;
                ex_nodes.push_back({ cls->name, cls });
            }

            ex_nodes.sort();

            std::string path("ram/skin_param/");
            path.append(buf);

            file_stream s;
            s.open(path.c_str(), "wb");

            key_val_out kv;
            s.write("# This file was generated automatically. DO NOT EDIT.\n", 54);

            for (auto& j : ex_nodes) {
                kv.open_scope(j.first.c_str());

                skin_param* skp = j.second->type == EX_OSAGE
                    ? ((ExOsageBlock*)j.second)->rob.skin_param_ptr
                    : ((ExClothBlock*)j.second)->rob.skin_param_ptr;

                ExOsageBlock* osg = j.second->type == EX_OSAGE
                    ? (ExOsageBlock*)j.second : 0;

                std::vector<int32_t> sort_index;
                int32_t* sort_index_data = 0;

                if (osg && osg->rob.nodes.size() > 1) {
                    RobOsageNode* rob_osg_node = osg->rob.nodes.data() + 1;
                    int32_t node_count = (int32_t)(osg->rob.nodes.size() - 1);

                    key_val_out::get_lexicographic_order(sort_index, node_count);
                    sort_index_data = sort_index.data();
                    kv.open_scope("node");
                    for (int32_t k = 0; k < node_count; k++) {
                        kv.open_scope_fmt(sort_index_data[k]);
                        skin_param_osage_node* node = &rob_osg_node[sort_index_data[k]].data_ptr->skp_osg_node;

                        kv.write(s, "coli_r", node->coli_r, flt_fmt);
                        kv.write(s, "hinge_ymax", node->hinge.ymax* RAD_TO_DEG_FLOAT, flt_fmt);
                        kv.write(s, "hinge_ymin", node->hinge.ymin* RAD_TO_DEG_FLOAT, flt_fmt);
                        kv.write(s, "hinge_zmax", node->hinge.zmax* RAD_TO_DEG_FLOAT, flt_fmt);
                        kv.write(s, "hinge_zmin", node->hinge.zmin* RAD_TO_DEG_FLOAT, flt_fmt);
                        kv.write(s, "inertial_cancel", node->inertial_cancel, flt_fmt);
                        kv.write(s, "weight", node->weight, flt_fmt);

                        kv.close_scope();
                    }

                    kv.write(s, "length", node_count);
                    kv.close_scope();
                }
                else {
                    kv.open_scope("node");
                    kv.write(s, "length", 0);
                    kv.close_scope();
                }

                kv.open_scope("root");

                if (skp->air_res != 1.0f)
                    kv.write(s, "air_res", skp->air_res, flt_fmt);

                auto elem_boc = bocs.find(osg);
                if (elem_boc != bocs.end()) {
                    prj::vector<skin_param_osage_root_boc>& vb = elem_boc->second;
                    int32_t boc_count = (int32_t)vb.size();

                    key_val_out::get_lexicographic_order(sort_index, boc_count);
                    sort_index_data = sort_index.data();
                    kv.open_scope("boc");
                    for (int32_t k = 0; k < boc_count; k++) {
                        kv.open_scope_fmt(sort_index_data[k]);
                        skin_param_osage_root_boc* boc = &vb[sort_index_data[k]];

                        kv.write(s, "ed_node", boc->ed_node);
                        kv.write(s, "ed_root", boc->ed_root.c_str());
                        kv.write(s, "st_node", boc->st_node);

                        kv.close_scope();
                    }

                    kv.write(s, "length", boc_count);
                    kv.close_scope();
                }
                else {
                    kv.open_scope("boc");
                    kv.write(s, "length", 0);
                    kv.close_scope();
                }

                prj::vector<SkinParam::CollisionParam>& vc = skp->coli;

                int32_t coli_count = 0;
                for (SkinParam::CollisionParam& k : vc)
                    if (k.type)
                        coli_count++;
                    else
                        break;

                kv.open_scope("coli");

                key_val_out::get_lexicographic_order(sort_index, coli_count);
                sort_index_data = sort_index.data();
                for (int32_t k = 0; k < coli_count; k++) {
                    kv.open_scope_fmt(sort_index_data[k]);
                    SkinParam::CollisionParam* cls_param = &vc[sort_index_data[k]];

                    kv.write(s, "bone.0.name", skeleton->object_bone_names[cls_param->node_idx[0]]);
                    kv.write(s, "bone.0.posx", cls_param->pos[0].x, flt_fmt);
                    kv.write(s, "bone.0.posy", cls_param->pos[0].y, flt_fmt);
                    kv.write(s, "bone.0.posz", cls_param->pos[0].z, flt_fmt);

                    switch (cls_param->type) {
                    case SkinParam::CollisionTypeCapsule:
                    case SkinParam::CollisionTypePlane:
                    case SkinParam::CollisionTypeEllipse:
                        kv.write(s, "bone.1.name", skeleton->object_bone_names[cls_param->node_idx[1]]);
                        kv.write(s, "bone.1.posx", cls_param->pos[1].x, flt_fmt);
                        kv.write(s, "bone.1.posy", cls_param->pos[1].y, flt_fmt);
                        kv.write(s, "bone.1.posz", cls_param->pos[1].z, flt_fmt);
                        break;
                    }

                    kv.write(s, "radius", cls_param->radius, flt_fmt);
                    kv.write(s, "type", cls_param->type);

                    kv.close_scope();
                }

                kv.write(s, "length", coli_count);
                kv.close_scope();

                if (skp->coli_r != 0.0f)
                    kv.write(s, "coli_r", skp->coli_r, flt_fmt);

                kv.write(s, "coli_type", skp->coli_type);

                if (skp->colli_tgt_osg)
                    for (ExOsageBlock*& k : itm_eq_obj->osage_blocks)
                        if (skp->colli_tgt_osg == &k->rob.nodes) {
                            kv.write(s, "colli_tgt_osg", k->name);
                            break;
                        }

                kv.write(s, "force", skp->force, flt_fmt);
                kv.write(s, "force_gain", skp->force_gain, flt_fmt);
                kv.write(s, "friction", skp->friction, flt_fmt);

                if (fabsf(skp->hinge.ymax * RAD_TO_DEG_FLOAT - 90.0f) > 0.0001f
                    || fabsf(skp->hinge.zmax * RAD_TO_DEG_FLOAT - 90.0f) > 0.0001f) {
                    kv.write(s, "hinge_y", skp->hinge.ymax, flt_fmt);
                    kv.write(s, "hinge_z", skp->hinge.zmax, flt_fmt);
                }

                kv.write(s, "init_rot_y", skp->init_rot.y * RAD_TO_DEG_FLOAT, flt_fmt);
                kv.write(s, "init_rot_z", skp->init_rot.z * RAD_TO_DEG_FLOAT, flt_fmt);

                auto elem_normal_ref = normal_refs.find(osg);
                if (elem_normal_ref != normal_refs.end()) {
                    prj::vector<skin_param_osage_root_normal_ref>& vnr = elem_normal_ref->second;
                    int32_t normal_ref_count = (int32_t)vnr.size();

                    key_val_out::get_lexicographic_order(sort_index, normal_ref_count);
                    sort_index_data = sort_index.data();
                    kv.open_scope("normal_ref");
                    for (int32_t k = 0; k < normal_ref_count; k++) {
                        kv.open_scope_fmt(sort_index_data[k]);
                        skin_param_osage_root_normal_ref* normal_ref = &vnr[sort_index_data[k]];

                        if (normal_ref->d.size())
                            kv.write(s, "D", normal_ref->d.c_str());

                        if (normal_ref->l.size())
                            kv.write(s, "L", normal_ref->l.c_str());

                        kv.write(s, "N", normal_ref->n.c_str());

                        if (normal_ref->r.size())
                            kv.write(s, "R", normal_ref->r.c_str());

                        if (normal_ref->u.size())
                            kv.write(s, "U", normal_ref->u.c_str());

                        kv.close_scope();
                    }

                    kv.write(s, "length", normal_ref_count);
                    kv.close_scope();
                }

                if (skp->move_cancel != -0.01f)
                    kv.write(s, "move_cancel", skp->move_cancel, flt_fmt);

                kv.write(s, "rot_y", skp->rot.y * RAD_TO_DEG_FLOAT, flt_fmt);
                kv.write(s, "rot_z", skp->rot.z * RAD_TO_DEG_FLOAT, flt_fmt);

                if (skp->stiffness != 0.0f)
                    kv.write(s, "stiffness", skp->stiffness, flt_fmt);

                kv.write(s, "wind_afc", skp->wind_afc, flt_fmt);

                kv.close_scope();

                kv.close_scope();
            }

            s.close();
        }
    }

    return false;
}

bool RobOsageTest::dest() {
    rob_osage_test_dw->SetDisp();
    return true;
}

void RobOsageTest::disp() {
    if (chara_id < 0 || chara_id >= ROB_CHARA_COUNT)
        return;

    rob_chara* rob_chr = rob_chara_array_get(get_rob_chara_smth(), chara_id);
    if (rob_chr && !pv_osage_manager_array_get_disp(&chara_id)
        && rob_chr->is_visible() && !(rob_chr->data.field_3 & 0x80)) {
        disp_coli();
        disp_line();
    }
}

void RobOsageTest::basic() {
    if (collision_update) {
        collision_update = false;

        rob_osage_test_dw->collision.Update();
        rob_osage_test_dw->colli_element.Update();
        rob_osage_test_dw->UpdateLayout();
    }

    rob_chara_item_equip_object* itm_eq_obj = get_item_equip_object();
    if (!itm_eq_obj)
        return;

    ExOsageBlock* osg = get_osage_block(itm_eq_obj);
    if (osg) {
        osg->rob.SetForce(root.force, root.gain);
        osg->rob.SetAirRes(root.air_res);
        osg->rob.SetRot(root.root_y_rot, root.root_z_rot);
        osg->rob.skin_param_ptr->friction = root.fric;
        osg->rob.skin_param_ptr->wind_afc = root.wind_afc;
        osg->rob.skin_param_ptr->coli_type = root.coli_type;
        osg->rob.SetInitRot(root.init_y_rot, root.init_z_rot);
    }

    ExClothBlock* cls = get_cloth_block(itm_eq_obj);
    if (cls) {
        skin_param* skp = cls->rob.skin_param_ptr;
        skp->force = root.force;
        skp->force_gain = root.gain;
        skp->air_res = root.air_res;
        skp->rot.y = root.root_y_rot * DEG_TO_RAD_FLOAT;
        skp->rot.z = root.root_z_rot * DEG_TO_RAD_FLOAT;
        skp->friction = root.fric;
        skp->wind_afc = root.wind_afc;
        skp->coli_type = root.coli_type;
        skp->init_rot.y = root.init_y_rot * DEG_TO_RAD_FLOAT;
        skp->init_rot.z = root.init_z_rot * DEG_TO_RAD_FLOAT;
    }
}

void RobOsageTest::disp_coli() {
    if (!coli)
        return;

    rob_chara_item_equip_object* itm_eq_obj = get_item_equip_object();
    if (!itm_eq_obj)
        return;

    prj::vector<SkinParam::CollisionParam>* cls_list = 0;
    mat4* transform = 0;

    ExOsageBlock* osg = get_osage_block(itm_eq_obj);
    ExClothBlock* cls = 0;
    if (osg) {
        cls_list = get_cls_list(osg);
        transform = osg->mats;
    }
    else {
        cls = get_cloth_block(itm_eq_obj);
        if (cls) {
            cls_list = get_cls_list(cls);
            transform = cls->mats;
        }
    }

    if (!cls_list || !transform)
        return;

    static const color4u8 selected_color = 0xCF00EF00;
    static const color4u8 cls_node_color = 0xCFEFEF00;
    static const color4u8 osg_node_color = 0xCFEFEF00;
    static const color4u8 default_color  = 0xCFFFFFFF;

    SkinParam::CollisionParam* cls_param = get_cls_param(cls_list);
    for (const SkinParam::CollisionParam& i : *cls_list)
        switch (i.type) {
        case SkinParam::CollisionTypeBall: {
            mdl::EtcObj etc(mdl::ETC_OBJ_SPHERE);
            etc.color = cls_param && &i == cls_param ? selected_color : default_color;
            etc.constant = true;

            etc.data.sphere.radius = i.radius;
            etc.data.sphere.slices = 16;
            etc.data.sphere.stacks = 16;
            etc.data.sphere.wire = false;

            vec3 pos;
            mat4 mat;
            mat4_transpose(&transform[i.node_idx[0]], &mat);
            mat4_transform_point(&mat, &i.pos[0], &pos);

            mat4_translate(&pos, &mat);
            mat4_transpose(&mat, &mat);
            disp_manager->entry_obj_etc(&mat, &etc);
        } break;
        case SkinParam::CollisionTypeCapsule: {
            mdl::EtcObj etc(mdl::ETC_OBJ_CAPSULE);
            etc.color = cls_param && &i == cls_param ? selected_color : default_color;
            etc.constant = true;

            etc.data.capsule.radius = i.radius;
            etc.data.capsule.slices = 16;
            etc.data.capsule.stacks = 16;
            etc.data.capsule.wire = false;

            mat4 mat;
            mat4_transpose(&transform[i.node_idx[0]], &mat);
            mat4_transform_point(&mat, &i.pos[0], &etc.data.capsule.pos[0]);
            mat4_transpose(&transform[i.node_idx[1]], &mat);
            mat4_transform_point(&mat, &i.pos[1], &etc.data.capsule.pos[1]);
            disp_manager->entry_obj_etc(&mat4_identity, &etc);
        } break;
        case SkinParam::CollisionTypePlane: {
            mdl::EtcObj etc(mdl::ETC_OBJ_PLANE);
            etc.color = cls_param && &i == cls_param ? selected_color : default_color;
            etc.constant = true;

            etc.data.plane.w = 2;
            etc.data.plane.h = 2;

            vec3 pos;
            mat4 mat;
            mat4_transpose(&transform[i.node_idx[0]], &mat);
            mat4_transform_point(&mat, &i.pos[0], &pos);

            vec3 dir = vec3::normalize(i.pos[1]);
            vec3 up = { 0.0f, 1.0f, 0.0f };
            vec3 axis;
            float_t angle;
            Glitter::axis_angle_from_vectors(&axis, &angle, &up, &dir);

            mat4 mat_rot = mat4_identity;
            mat4_mul_rotation(&mat_rot, &axis, angle, &mat_rot);

            mat4_translate(&pos, &mat);
            mat4_mul(&mat_rot, &mat, &mat);
            mat4_scale_rot(&mat, i.radius, &mat);
            mat4_transpose(&mat, &mat);
            disp_manager->entry_obj_etc(&mat, &etc);
        } break;
        case SkinParam::CollisionTypeEllipse: {
            mdl::EtcObj etc(mdl::ETC_OBJ_SPHERE);
            etc.color = cls_param && &i == cls_param ? selected_color : default_color;
            etc.constant = true;

            etc.data.sphere.radius = 1.0f;
            etc.data.sphere.slices = 16;
            etc.data.sphere.stacks = 16;
            etc.data.sphere.wire = false;

            vec3 pos[2];
            mat4 mat;
            mat4_transpose(&transform[i.node_idx[0]], &mat);
            mat4_transform_point(&mat, &i.pos[0], &pos[0]);
            mat4_transpose(&transform[i.node_idx[1]], &mat);
            mat4_transform_point(&mat, &i.pos[1], &pos[1]);

            vec3 origin = (pos[0] + pos[1]) * 0.5f;
            mat4_translate(&origin, &mat);

            vec3 dir = vec3::normalize(pos[1] - pos[0]);
            vec3 up = { 0.0f, 1.0f, 0.0f };
            vec3 axis;
            float_t angle;
            Glitter::axis_angle_from_vectors(&axis, &angle, &up, &dir);

            mat4 m = mat4_identity;
            mat4_mul_rotation(&m, &axis, angle, &m);
            mat4_mul(&m, &mat, &mat);

            const float_t length = vec3::length((pos[0] - pos[1]) * 0.5f);
            const float_t scale_y = sqrtf(i.radius * i.radius + length * length);

            mat4_scale_rot(&mat, i.radius, scale_y, i.radius, &mat);
            mat4_transpose(&mat, &mat);
            disp_manager->entry_obj_etc(&mat, &etc);
        } break;
        case SkinParam::CollisionTypeAABB: {
            mdl::EtcObj etc(mdl::ETC_OBJ_CUBE);
            etc.color = cls_param && &i == cls_param ? selected_color : default_color;
            etc.constant = true;

            vec3 pos;
            mat4 mat;
            mat4_transpose(&transform[i.node_idx[0]], &mat);
            mat4_transform_point(&mat, &i.pos[0], &pos);

            etc.data.cube.size = i.radius;
            etc.data.cube.wire = false;

            mat4_translate(&pos, &mat);
            mat4_transpose(&mat, &mat);
            disp_manager->entry_obj_etc(&mat, &etc);
        } break;
        }

    if (osg && osg->rob.nodes.size() > 1) {
        RobOsageNode* i_begin = osg->rob.nodes.data() + 1;
        RobOsageNode* i_end = osg->rob.nodes.data() + osg->rob.nodes.size();
        for (RobOsageNode* i = i_begin; i != i_end; i++) {
            mdl::EtcObj etc(mdl::ETC_OBJ_CAPSULE);
            etc.color = osg_node_color;
            etc.constant = true;

            etc.data.capsule.radius = i->data_ptr->skp_osg_node.coli_r;
            etc.data.capsule.slices = 16;
            etc.data.capsule.stacks = 16;
            etc.data.capsule.wire = false;
            etc.data.capsule.pos[0] = i[-1].pos;
            etc.data.capsule.pos[1] = i[ 0].pos;
            disp_manager->entry_obj_etc(&mat4_identity, &etc);
        }
    }

    if (cls && cls->rob.nodes.size() > 1) {
        CLOTHNode* i_begin = cls->rob.nodes.data() + cls->rob.root_count;
        CLOTHNode* i_end = cls->rob.nodes.data() + cls->rob.nodes.size();
        for (CLOTHNode* i = i_begin; i != i_end; i++) {
            mdl::EtcObj etc(mdl::ETC_OBJ_SPHERE);
            etc.color = cls_node_color;
            etc.constant = true;

            etc.data.sphere.radius = max_def(cls->rob.skin_param_ptr->coli_r, 0.005f);
            etc.data.sphere.slices = 16;
            etc.data.sphere.stacks = 16;
            etc.data.sphere.wire = false;

            vec3 pos;
            mat4 mat;
            mat4_transpose(&itm_eq_obj->item_equip->mat, &mat);
            mat4_transform_point(&mat, &i->pos, &pos);

            mat4_translate(&pos, &mat);
            mat4_transpose(&mat, &mat);
            disp_manager->entry_obj_etc(&mat, &etc);
        }
    }
}

void RobOsageTest::disp_line() {
    if (!line)
        return;

}

inline ExClothBlock* RobOsageTest::get_cloth_block(rob_chara_item_equip_object* itm_eq_obj) const {
    if (itm_eq_obj && itm_eq_obj->obj_info == obj_info && osage_index >= itm_eq_obj->osage_blocks.size()
        && osage_index - itm_eq_obj->osage_blocks.size() < itm_eq_obj->cloth_blocks.size())
        return itm_eq_obj->cloth_blocks[osage_index - itm_eq_obj->osage_blocks.size()];
    return 0;
}

inline rob_chara_item_equip_object* RobOsageTest::get_item_equip_object() const {
    if (chara_id < 0 || chara_id >= ROB_CHARA_COUNT)
        return 0;

    rob_chara_item_equip* rob_itm_equip = rob_chara_array_get_item_equip(get_rob_chara_smth(), chara_id);
    if (rob_itm_equip && item_id >= ITEM_BODY && item_id < ITEM_MAX)
        return rob_itm_equip->get_item_equip_object(item_id);
    return 0;
}

inline ExOsageBlock* RobOsageTest::get_osage_block(rob_chara_item_equip_object* itm_eq_obj) const {
    if (itm_eq_obj && itm_eq_obj->obj_info == obj_info && osage_index < itm_eq_obj->osage_blocks.size())
        return itm_eq_obj->osage_blocks[osage_index];
    return 0;
}

inline prj::vector<SkinParam::CollisionParam>* RobOsageTest::get_cls_list(ExClothBlock* cls) const {
    if (cls)
        return &cls->rob.skin_param_ptr->coli;
    return 0;
}

inline prj::vector<SkinParam::CollisionParam>* RobOsageTest::get_cls_list(ExOsageBlock* osg) const {
    if (osg)
        return &osg->rob.skin_param_ptr->coli;
    return 0;
}

inline SkinParam::CollisionParam* RobOsageTest::get_cls_param(
    prj::vector<SkinParam::CollisionParam>* cls_list) const {
    if (cls_list && collision_index < cls_list->size())
        return &cls_list->data()[collision_index];
    return 0;
}

void RobOsageTest::set_node(skin_param_osage_node* skp_osg_node) {
    if (!skp_osg_node)
        return;

    node.coli_r = skp_osg_node->coli_r;
    node.weight = skp_osg_node->weight;
    node.inertial_cancel = skp_osg_node->inertial_cancel;
    node.hinge.ymin = skp_osg_node->hinge.ymin * RAD_TO_DEG_FLOAT;
    node.hinge.ymax = skp_osg_node->hinge.ymax * RAD_TO_DEG_FLOAT;
    node.hinge.zmin = skp_osg_node->hinge.zmin * RAD_TO_DEG_FLOAT;
    node.hinge.zmax = skp_osg_node->hinge.zmax * RAD_TO_DEG_FLOAT;
}

void RobOsageTest::set_root(skin_param* skp) {
    if (!skp)
        return;

    root.force = skp->force;
    root.gain = skp->force_gain;
    root.air_res = skp->air_res;
    root.root_y_rot = skp->rot.y * RAD_TO_DEG_FLOAT;
    root.root_z_rot = skp->rot.z * RAD_TO_DEG_FLOAT;
    root.fric = skp->friction;
    root.wind_afc = skp->wind_afc;
    root.coli_type = skp->coli_type;
    root.init_y_rot = skp->init_rot.y * RAD_TO_DEG_FLOAT;
    root.init_z_rot = skp->init_rot.z * RAD_TO_DEG_FLOAT;
}

HOOK(bool, FASTCALL, SubGameState__DataTestMot__Dest, 0x00000001402871F0) {
    rob_osage_test->del();
    return originalSubGameState__DataTestMot__Dest();
}

HOOK(bool, FASTCALL, SubGameState__DataTestMot__Init, 0x00000001402876B0) {
    app::TaskWork::add_task(rob_osage_test, "ROB_OSAGE_TEST");
    return originalSubGameState__DataTestMot__Init();
}

void rob_osage_test_init() {
    if (!rob_osage_test)
        rob_osage_test = new RobOsageTest;

    INSTALL_HOOK(SubGameState__DataTestMot__Dest);
    INSTALL_HOOK(SubGameState__DataTestMot__Init);
}

void rob_osage_test_free() {
    if (rob_osage_test) {
        delete rob_osage_test;
        rob_osage_test = 0;
    }
}

void RobOsageTestDw::Rob::Init(dw::Composite* parent) {
    dw::Composite* comp = new dw::Composite(parent);

    dw::Composite* ribbon_comp = new dw::Composite(comp);
    ribbon_comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    list_box = new dw::ListBox(ribbon_comp);
    list_box->AddItem("1P");
    list_box->AddItem("2P");
    list_box->AddItem("3P");
    list_box->AddItem("4P");
    list_box->AddItem("5P");
    list_box->AddItem("6P");
    list_box->SetItemIndex(0);
    list_box->AddSelectionListener(new dw::SelectionListenerOnHook(RobOsageTestDw::Rob::CharaCallback));

    save_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    save_button->SetText("  SAVE  ");
    save_button->callback = RobOsageTestDw::Rob::SaveCallback;

    load_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    load_button->SetText("  LOAD  ");
    load_button->callback = RobOsageTestDw::Rob::LoadCallback;

    default_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    default_button->SetText("DEFAULT");
    default_button->callback = RobOsageTestDw::Rob::DefaultCallback;

    object_list_box = new dw::ListBox(comp);
    object_list_box->AddSelectionListener(new dw::SelectionListenerOnHook(
        RobOsageTestDw::Rob::ObjectCallback));
}

void RobOsageTestDw::Rob::CharaCallback(dw::Widget* data) {
    dw::ListBox* list_box = dynamic_cast<dw::ListBox*>(data);
    if (list_box)
        rob_osage_test->load_chara_id = (int32_t)list_box->list->selected_item;
}

void RobOsageTestDw::Rob::DefaultCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button) {
        rob_osage_test->chara_id = -1;
        rob_osage_test->load_chara_id = 0;
        rob_osage_test->item_id = ITEM_NONE;
        rob_osage_test->obj_info = object_info();
        rob_osage_test->osage_index = -1;
        rob_osage_test->collision_index = -1;
        rob_osage_test->collision_update = true;

        rob_osage_test->root = {};
        rob_osage_test->node = {};

        rob_osage_test_dw->root.Update();
        rob_osage_test_dw->node.Update();
        rob_osage_test_dw->colli_element.Update();
    }
}

void RobOsageTestDw::Rob::LoadCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button)
        rob_osage_test->load = true;
}

void RobOsageTestDw::Rob::ObjectCallback(dw::Widget* data) {
    dw::ListBox* list_box = dynamic_cast<dw::ListBox*>(data);
    if (list_box && list_box->list->selected_item < rob_osage_test->objects.size()) {
        auto elem = rob_osage_test->objects.data()[list_box->list->selected_item];
        rob_osage_test->item_id = elem.first;
        rob_osage_test->obj_info = elem.second;
        rob_osage_test->osage_index = -1;
        rob_osage_test->collision_index = -1;
        rob_osage_test->set_root();
        rob_osage_test->set_node();

        rob_osage_test_dw->root.list_box->ClearItems();

        rob_chara_item_equip_object* itm_eq_obj = rob_osage_test->get_item_equip_object();
        if (itm_eq_obj) {
            for (ExOsageBlock*& i : itm_eq_obj->osage_blocks)
                rob_osage_test_dw->root.list_box->AddItem(i->name);

            for (ExClothBlock*& i : itm_eq_obj->cloth_blocks)
                rob_osage_test_dw->root.list_box->AddItem(i->name);
        }
    }
}

void RobOsageTestDw::Rob::SaveCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button)
        rob_osage_test->save = true;
}

void RobOsageTestDw::Flags::Init(dw::Composite* parent) {
    dw::Composite* comp = new dw::Composite(parent);
    comp->SetLayout(new dw::GridLayout(3));

    line_button = new dw::Button(comp, dw::CHECKBOX);
    line_button->SetText("Line   ");
    line_button->SetValue(rob_osage_test->line);
    line_button->callback = RobOsageTestDw::Flags::LineCallback;

    lock_button = new dw::Button(comp, dw::CHECKBOX);
    lock_button->SetText("Lock   ");
    lock_button->callback = RobOsageTestDw::Flags::LockCallback;

    no_pause_button = new dw::Button(comp, dw::CHECKBOX);
    no_pause_button->SetText("NoPause");
    no_pause_button->callback = RobOsageTestDw::Flags::NoPauseCallback;

    coli_button = new dw::Button(comp, dw::CHECKBOX);
    coli_button->SetText("Coli   ");
    coli_button->SetValue(rob_osage_test->coli);
    coli_button->callback = RobOsageTestDw::Flags::ColiCallback;

    name_button = new dw::Button(comp, dw::CHECKBOX);
    name_button->SetText("Name   ");
    name_button->callback = RobOsageTestDw::Flags::NameCallback;

    init_button = new dw::Button(comp, dw::CHECKBOX);
    init_button->SetText("Init   ");
    init_button->callback = RobOsageTestDw::Flags::InitCallback;
}

void RobOsageTestDw::Flags::ColiCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button)
        rob_osage_test->coli = button->value;
}

void RobOsageTestDw::Flags::InitCallback(dw::Widget* data) {

}

void RobOsageTestDw::Flags::LineCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button)
        rob_osage_test->line = button->value;
}

void RobOsageTestDw::Flags::LockCallback(dw::Widget* data) {

}

void RobOsageTestDw::Flags::NameCallback(dw::Widget* data) {

}

void RobOsageTestDw::Flags::NoPauseCallback(dw::Widget* data) {

}

RobOsageTestDw::Root::Force::Force(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("force    ");
    button->SetValue(true);
    button->AddSelectionListener(this);

    Update(true);
}

RobOsageTestDw::Root::Force::~Force() {

}

void RobOsageTestDw::Root::Force::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Root::Force::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Root::Force::Update(bool value) {
    if (value) {
        if (slider)
            return;

        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 2.3f";
        slider->SetParams(rob_osage_test->root.force, 0.0f, 0.1f, 0.01f, 0.001f, 0.01f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Root::Force::Callback));
    }
    else {
        if (!slider)
            return;

        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Root::Force::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider)
        rob_osage_test->root.force = slider->scroll_bar->value;
}

RobOsageTestDw::Root::Gain::Gain(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("gain     ");
    button->SetValue(true);
    button->AddSelectionListener(this);

    Update(true);
}

RobOsageTestDw::Root::Gain::~Gain() {

}

void RobOsageTestDw::Root::Gain::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Root::Gain::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Root::Gain::Update(bool value) {
    if (value) {
        if (slider)
            return;

        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 2.3f";
        slider->SetParams(rob_osage_test->root.gain, 0.0f, 1.0f, 0.1f, 0.01f, 0.1f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Root::Gain::Callback));
    }
    else {
        if (!slider)
            return;

        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Root::Gain::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider)
        rob_osage_test->root.gain = slider->scroll_bar->value;
}

RobOsageTestDw::Root::AirRes::AirRes(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("air res  ");
    button->SetValue(true);
    button->AddSelectionListener(this);

    Update(true);
}

RobOsageTestDw::Root::AirRes::~AirRes() {

}

void RobOsageTestDw::Root::AirRes::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Root::AirRes::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Root::AirRes::Update(bool value) {
    if (value) {
        if (slider)
            return;

        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 2.3f";
        slider->SetParams(rob_osage_test->root.air_res, 0.0f, 1.0f, 0.1f, 0.01f, 0.1f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Root::AirRes::Callback));
    }
    else {
        if (!slider)
            return;

        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Root::AirRes::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider)
        rob_osage_test->root.air_res = slider->scroll_bar->value;
}

RobOsageTestDw::Root::RootYRot::RootYRot(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("rootYrot ");
    button->SetValue(true);
    button->AddSelectionListener(this);

    Update(true);
}

RobOsageTestDw::Root::RootYRot::~RootYRot() {

}

void RobOsageTestDw::Root::RootYRot::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Root::RootYRot::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Root::RootYRot::Update(bool value) {
    if (value) {
        if (slider)
            return;

        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 6.0f";
        slider->SetParams(rob_osage_test->root.root_y_rot,
            -180.0f, 180.0f, 90.0f, 1.0f, 10.0f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Root::RootYRot::Callback));
    }
    else {
        if (!slider)
            return;

        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Root::RootYRot::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider)
        rob_osage_test->root.root_y_rot = slider->scroll_bar->value;
}

RobOsageTestDw::Root::RootZRot::RootZRot(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("rootZrot ");
    button->SetValue(true);
    button->AddSelectionListener(this);

    Update(true);
}

RobOsageTestDw::Root::RootZRot::~RootZRot() {

}

void RobOsageTestDw::Root::RootZRot::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Root::RootZRot::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Root::RootZRot::Update(bool value) {
    if (value) {
        if (slider)
            return;

        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 6.0f";
        slider->SetParams(rob_osage_test->root.root_z_rot,
            -180.0f, 180.0f, 90.0f, 1.0f, 10.0f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Root::RootZRot::Callback));
    }
    else {
        if (!slider)
            return;

        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Root::RootZRot::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider)
        rob_osage_test->root.root_z_rot = slider->scroll_bar->value;
}

RobOsageTestDw::Root::Fric::Fric(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("fric     ");
    button->SetValue(true);
    button->AddSelectionListener(this);

    Update(true);
}

RobOsageTestDw::Root::Fric::~Fric() {

}

void RobOsageTestDw::Root::Fric::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Root::Fric::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Root::Fric::Update(bool value) {
    if (value) {
        if (slider)
            return;

        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 2.3f";
        slider->SetParams(rob_osage_test->root.fric, 0.0f, 1.0f, 0.1f, 0.01f, 0.1f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Root::Fric::Callback));
    }
    else {
        if (!slider)
            return;

        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Root::Fric::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider)
        rob_osage_test->root.fric = slider->scroll_bar->value;
}

RobOsageTestDw::Root::WindAfc::WindAfc(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("windAfc  ");
    button->SetValue(true);
    button->AddSelectionListener(this);

    Update(true);
}

RobOsageTestDw::Root::WindAfc::~WindAfc() {

}

void RobOsageTestDw::Root::WindAfc::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Root::WindAfc::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Root::WindAfc::Update(bool value) {
    if (value) {
        if (slider)
            return;

        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 2.3f";
        slider->SetParams(rob_osage_test->root.wind_afc, 0.0f, 1.0f, 0.1f, 0.01f, 0.1f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Root::WindAfc::Callback));
    }
    else {
        if (!slider)
            return;

        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Root::WindAfc::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider)
        rob_osage_test->root.wind_afc = slider->scroll_bar->value;
}

RobOsageTestDw::Root::ColiType::ColiType(dw::Composite* parent) : list_box() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("");
    button->SetValue(true);
    button->AddSelectionListener(this);

    Update(true);
}

RobOsageTestDw::Root::ColiType::~ColiType() {

}

void RobOsageTestDw::Root::ColiType::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Root::ColiType::SetItemIndex(size_t value) {
    if (list_box)
        list_box->SetItemIndex(value);
}

void RobOsageTestDw::Root::ColiType::Update(bool value) {
    if (value) {
        if (list_box)
            return;

        list_box = new dw::ListBox(comp, dw::MULTISELECT);
        list_box->AddItem("coli_type: End");
        list_box->AddItem("coli_type: Ball");
        list_box->AddItem("coli_type: Capsule(+root)");
        list_box->SetItemIndex(rob_osage_test->root.coli_type);
        list_box->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Root::ColiType::Callback));
    }
    else {
        if (!list_box)
            return;

        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(list_box));
        list_box->Reset();
        list_box = 0;
    }
}

void RobOsageTestDw::Root::ColiType::Callback(dw::Widget* data) {
    dw::ListBox* list_box = dynamic_cast<dw::ListBox*>(data);
    if (list_box)
        rob_osage_test->root.coli_type = (SkinParam::RootCollisionType)(int32_t)list_box->list->selected_item;
}

RobOsageTestDw::Root::InitYRot::InitYRot(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("initYrot ");
    button->SetValue(true);
    button->AddSelectionListener(this);

    Update(true);
}

RobOsageTestDw::Root::InitYRot::~InitYRot() {

}

void RobOsageTestDw::Root::InitYRot::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Root::InitYRot::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Root::InitYRot::Update(bool value) {
    if (value) {
        if (slider)
            return;

        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 6.0f";
        slider->SetParams(rob_osage_test->root.init_y_rot,
            -180.0f, 180.0f, 90.0f, 1.0f, 10.0f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Root::InitYRot::Callback));
    }
    else {
        if (!slider)
            return;

        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Root::InitYRot::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider)
        rob_osage_test->root.init_y_rot = slider->scroll_bar->value;
}

RobOsageTestDw::Root::InitZRot::InitZRot(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("initZrot ");
    button->SetValue(true);
    button->AddSelectionListener(this);

    Update(true);
}

RobOsageTestDw::Root::InitZRot::~InitZRot() {

}

void RobOsageTestDw::Root::InitZRot::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Root::InitZRot::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Root::InitZRot::Update(bool value) {
    if (value) {
        if (slider)
            return;

        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 6.0f";
        slider->SetParams(rob_osage_test->root.init_z_rot,
            -180.0f, 180.0f, 90.0f, 1.0f, 10.0f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Root::InitZRot::Callback));
    }
    else {
        if (!slider)
            return;

        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Root::InitZRot::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider)
        rob_osage_test->root.init_z_rot = slider->scroll_bar->value;
}

void RobOsageTestDw::Root::Init(dw::Composite* parent) {
    dw::Composite* comp = new dw::Composite(parent);

    list_box = new dw::ListBox(comp);
    list_box->AddSelectionListener(new dw::SelectionListenerOnHook(RobOsageTestDw::Root::OsageCallback));

    dw::Composite* ribbon_comp = new dw::Composite(comp);
    ribbon_comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    reset_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    reset_button->SetText("  RESET ");
    reset_button->callback = RobOsageTestDw::Root::ResetCallback;

    copy_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    copy_button->SetText("  COPY  ");
    copy_button->callback = RobOsageTestDw::Root::CopyCallback;

    paste_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    paste_button->SetText("  PASTE ");
    paste_button->callback = RobOsageTestDw::Root::PasteCallback;

    force = new Force(comp);
    gain = new Gain(comp);
    air_res = new AirRes(comp);
    root_y_rot = new RootYRot(comp);
    root_z_rot = new RootZRot(comp);
    fric = new Fric(comp);
    wind_afc = new WindAfc(comp);
    coli_type = new ColiType(comp);
    init_y_rot = new InitYRot(comp);
    init_z_rot = new InitZRot(comp);
}

void RobOsageTestDw::Root::Update() {
    const RobOsageTest::Root& root = rob_osage_test->root;

    force->SetValue(root.force);
    gain->SetValue(root.gain);
    air_res->SetValue(root.air_res);
    root_y_rot->SetValue(root.root_y_rot);
    root_z_rot->SetValue(root.root_z_rot);
    fric->SetValue(root.fric);
    wind_afc->SetValue(root.wind_afc);
    coli_type->SetItemIndex(root.coli_type);
    init_y_rot->SetValue(root.init_y_rot);
    init_z_rot->SetValue(root.init_z_rot);
}

void RobOsageTestDw::Root::CopyCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button) {
        rob_osage_test->root_temp = rob_osage_test->root;
        rob_osage_test->node_temp = rob_osage_test->node;
    }
}

void RobOsageTestDw::Root::OsageCallback(dw::Widget* data) {
    dw::ListBox* list_box = dynamic_cast<dw::ListBox*>(data);
    if (list_box) {
        rob_osage_test->osage_index = list_box->list->selected_item;
        rob_osage_test->collision_index = -1;
        rob_osage_test->collision_update = true;

        rob_osage_test->set_root();
        rob_osage_test->set_node();
        rob_osage_test_dw->root.Update();
        rob_osage_test_dw->node.Update();
    }
}

void RobOsageTestDw::Root::PasteCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button) {
        rob_osage_test->root = rob_osage_test->root_temp;
        rob_osage_test->node = rob_osage_test->node_temp;

        rob_osage_test_dw->root.Update();
    }
}

void RobOsageTestDw::Root::ResetCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button) {
        rob_osage_test->root = {};
        rob_osage_test->node = {};

        rob_osage_test_dw->root.Update();
        rob_osage_test_dw->node.Update();
    }
}

RobOsageTestDw::Node::CollisionRadius::CollisionRadius(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("coli_r  ");
    button->AddSelectionListener(this);

    Update(false);
}

RobOsageTestDw::Node::CollisionRadius::~CollisionRadius() {

}

void RobOsageTestDw::Node::CollisionRadius::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Node::CollisionRadius::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Node::CollisionRadius::Update(bool value) {
    if (value) {
        if (slider)
            return;

        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 2.3f";
        slider->SetParams(rob_osage_test->node.coli_r, 0.0f, 2.0f, 0.01f, 0.001f, 0.01f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Node::CollisionRadius::Callback));
    }
    else {
        if (!slider)
            return;

        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Node::CollisionRadius::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider) {
        rob_chara_item_equip_object* itm_eq_obj = rob_osage_test->get_item_equip_object();
        ExOsageBlock* osg = rob_osage_test->get_osage_block(itm_eq_obj);
        if (osg) {
            float_t value = slider->scroll_bar->value;
            RobOsageNode* i_begin = osg->rob.nodes.data() + 1;
            RobOsageNode* i_end = osg->rob.nodes.data() + osg->rob.nodes.size();
            for (RobOsageNode* i = i_begin; i != i_end; i++)
                i->data_ptr->skp_osg_node.coli_r = value;
        }
    }
}

RobOsageTestDw::Node::Hinge::Hinge(dw::Composite* parent)
    : y_min_slider(), y_max_slider(), z_min_slider(), z_max_slider() {
    comp = new dw::Composite(parent);

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("hinge   ");
    button->AddSelectionListener(this);

    Update(false);
}

RobOsageTestDw::Node::Hinge::~Hinge() {

}

void RobOsageTestDw::Node::Hinge::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Node::Hinge::SetValue(const RobOsageTest::Node::Hinge& value) {
    if (y_min_slider)
        y_min_slider->SetValue(value.ymin);

    if (y_max_slider)
        y_max_slider->SetValue(value.ymax);

    if (z_min_slider)
        z_min_slider->SetValue(value.zmin);

    if (z_max_slider)
        z_max_slider->SetValue(value.zmax);
}

void RobOsageTestDw::Node::Hinge::Update(bool value) {
    if (value) {
        y_min_slider = dw::Slider::Create(comp);
        y_min_slider->SetText("   Ymin    ");
        y_min_slider->format = "% 6.0f";
        y_min_slider->SetParams(rob_osage_test->node.hinge.ymin, -180.0f, 0.0f, 10.0f, 0.5f, 5.0f);
        y_min_slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Node::Hinge::YMinCallback));

        y_max_slider = dw::Slider::Create(comp);
        y_max_slider->SetText("   Ymax    ");
        y_max_slider->format = "% 6.0f";
        y_max_slider->SetParams(rob_osage_test->node.hinge.ymax, 0.0f, 180.0f, 10.0f, 0.5f, 5.0f);
        y_max_slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Node::Hinge::YMaxCallback));

        z_min_slider = dw::Slider::Create(comp);
        z_min_slider->SetText("   Zmin    ");
        z_min_slider->format = "% 6.0f";
        z_min_slider->SetParams(rob_osage_test->node.hinge.zmin, -180.0f, 0.0f, 10.0f, 0.5f, 5.0f);
        z_min_slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Node::Hinge::ZMinCallback));

        z_max_slider = dw::Slider::Create(comp);
        z_max_slider->SetText("   Zmax    ");
        z_max_slider->format = "% 6.0f";
        z_max_slider->SetParams(rob_osage_test->node.hinge.zmax, 0.0f, 180.0f, 10.0f, 0.5f, 5.0f);
        z_max_slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Node::Hinge::ZMaxCallback));
    }
    else if (y_min_slider) {
        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(y_min_slider));
        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(y_max_slider));
        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(z_min_slider));
        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(z_max_slider));

        y_min_slider->Reset();
        y_max_slider->Reset();
        z_min_slider->Reset();
        z_max_slider->Reset();
        y_min_slider = 0;
        y_max_slider = 0;
        z_min_slider = 0;
        z_max_slider = 0;
    }
}

void RobOsageTestDw::Node::Hinge::YMinCallback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider) {
        rob_chara_item_equip_object* itm_eq_obj = rob_osage_test->get_item_equip_object();
        ExOsageBlock* osg = rob_osage_test->get_osage_block(itm_eq_obj);
        if (osg) {
            float_t value = slider->scroll_bar->value * DEG_TO_RAD_FLOAT;
            RobOsageNode* i_begin = osg->rob.nodes.data() + 1;
            RobOsageNode* i_end = osg->rob.nodes.data() + osg->rob.nodes.size();
            for (RobOsageNode* i = i_begin; i != i_end; i++)
                i->data_ptr->skp_osg_node.hinge.ymin = value;
        }
    }
}

void RobOsageTestDw::Node::Hinge::YMaxCallback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider) {
        rob_chara_item_equip_object* itm_eq_obj = rob_osage_test->get_item_equip_object();
        ExOsageBlock* osg = rob_osage_test->get_osage_block(itm_eq_obj);
        if (osg) {
            float_t value = slider->scroll_bar->value * DEG_TO_RAD_FLOAT;
            RobOsageNode* i_begin = osg->rob.nodes.data() + 1;
            RobOsageNode* i_end = osg->rob.nodes.data() + osg->rob.nodes.size();
            for (RobOsageNode* i = i_begin; i != i_end; i++)
                i->data_ptr->skp_osg_node.hinge.ymax = value;
        }
    }
}

void RobOsageTestDw::Node::Hinge::ZMinCallback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider) {
        rob_chara_item_equip_object* itm_eq_obj = rob_osage_test->get_item_equip_object();
        ExOsageBlock* osg = rob_osage_test->get_osage_block(itm_eq_obj);
        if (osg) {
            float_t value = slider->scroll_bar->value * DEG_TO_RAD_FLOAT;
            RobOsageNode* i_begin = osg->rob.nodes.data() + 1;
            RobOsageNode* i_end = osg->rob.nodes.data() + osg->rob.nodes.size();
            for (RobOsageNode* i = i_begin; i != i_end; i++)
                i->data_ptr->skp_osg_node.hinge.zmin = value;
        }
    }
}

void RobOsageTestDw::Node::Hinge::ZMaxCallback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider) {
        rob_chara_item_equip_object* itm_eq_obj = rob_osage_test->get_item_equip_object();
        ExOsageBlock* osg = rob_osage_test->get_osage_block(itm_eq_obj);
        if (osg) {
            float_t value = slider->scroll_bar->value * DEG_TO_RAD_FLOAT;
            RobOsageNode* i_begin = osg->rob.nodes.data() + 1;
            RobOsageNode* i_end = osg->rob.nodes.data() + osg->rob.nodes.size();
            for (RobOsageNode* i = i_begin; i != i_end; i++)
                i->data_ptr->skp_osg_node.hinge.zmax = value;
        }
    }
}

RobOsageTestDw::Node::InertialCancel::InertialCancel(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("i.c.    ");
    button->AddSelectionListener(this);

    Update(false);
}

RobOsageTestDw::Node::InertialCancel::~InertialCancel() {

}

void RobOsageTestDw::Node::InertialCancel::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Node::InertialCancel::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Node::InertialCancel::Update(bool value) {
    if (value) {
        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 2.3f";
        slider->SetParams(rob_osage_test->node.inertial_cancel, 0.0f, 1.0f, 0.1f, 0.01f, 0.1f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Node::InertialCancel::Callback));
    }
    else if (slider) {
        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Node::InertialCancel::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider) {
        rob_chara_item_equip_object* itm_eq_obj = rob_osage_test->get_item_equip_object();
        ExOsageBlock* osg = rob_osage_test->get_osage_block(itm_eq_obj);
        if (osg) {
            float_t value = slider->scroll_bar->value;
            RobOsageNode* i_begin = osg->rob.nodes.data() + 1;
            RobOsageNode* i_end = osg->rob.nodes.data() + osg->rob.nodes.size();
            for (RobOsageNode* i = i_begin; i != i_end; i++)
                i->data_ptr->skp_osg_node.inertial_cancel = value;
        }
    }
}

RobOsageTestDw::Node::Weight::Weight(dw::Composite* parent) : slider() {
    comp = new dw::Composite(parent);
    comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("weight  ");
    button->AddSelectionListener(this);

    Update(false);
}

RobOsageTestDw::Node::Weight::~Weight() {

}

void RobOsageTestDw::Node::Weight::Callback(dw::SelectionListener::CallbackData* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data->widget);
    if (button) {
        Update(button->value);
        rob_osage_test_dw->UpdateLayout();
    }
}

void RobOsageTestDw::Node::Weight::SetValue(float_t value) {
    if (slider)
        slider->SetValue(value);
}

void RobOsageTestDw::Node::Weight::Update(bool value) {
    if (value) {
        slider = dw::Slider::Create(comp);
        slider->SetText("");
        slider->format = "% 2.3f";
        slider->SetParams(rob_osage_test->node.weight, 0.0f, 1.0f, 0.1f, 0.01f, 0.1f);
        slider->AddSelectionListener(new dw::SelectionListenerOnHook(
            RobOsageTestDw::Node::Weight::Callback));
    }
    else if (slider) {
        comp->controls.erase(comp->controls.begin() + comp->GetControlIndex(slider));
        slider->Reset();
        slider = 0;
    }
}

void RobOsageTestDw::Node::Weight::Callback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider) {
        rob_chara_item_equip_object* itm_eq_obj = rob_osage_test->get_item_equip_object();
        ExOsageBlock* osg = rob_osage_test->get_osage_block(itm_eq_obj);
        if (osg) {
            float_t value = slider->scroll_bar->value;
            RobOsageNode* i_begin = osg->rob.nodes.data() + 1;
            RobOsageNode* i_end = osg->rob.nodes.data() + osg->rob.nodes.size();
            for (RobOsageNode* i = i_begin; i != i_end; i++)
                i->data_ptr->skp_osg_node.weight = value;
        }
    }
}

void RobOsageTestDw::Node::Init(dw::Composite* parent) {
    group = new dw::Group(parent);
    group->SetText("Node Param");

    coli_r = new CollisionRadius(group);
    weight = new Weight(group);
    inertial_cancel = new InertialCancel(group);
    hinge = new Hinge(group);
}

void RobOsageTestDw::Node::Update() {
    const RobOsageTest::Node& node = rob_osage_test->node;

    coli_r->SetValue(node.coli_r);
    weight->SetValue(node.weight);
    inertial_cancel->SetValue(node.inertial_cancel);
    hinge->SetValue(node.hinge);
}

void RobOsageTestDw::BetweenOsageCollision::Init(dw::Composite* parent) {
    dw::Composite* comp = new dw::Composite(parent);

    button = new dw::Button(comp, dw::CHECKBOX);
    button->SetText("Between Osage Collision");
}

void RobOsageTestDw::Collision::Init(dw::Composite* parent) {
    group = new dw::Group(parent);
    group->SetText("COLLISION");

    dw::Composite* ribbon_comp = new dw::Composite(group);
    ribbon_comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    reset_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    reset_button->SetText("  RESET ");
    reset_button->callback = RobOsageTestDw::Collision::ResetCallback;

    copy_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    copy_button->SetText("  COPY  ");
    copy_button->callback = RobOsageTestDw::Collision::CopyCallback;

    paste_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    paste_button->SetText("  PASTE ");
    paste_button->callback = RobOsageTestDw::Collision::PasteCallback;

    element_comp = 0;
}

void RobOsageTestDw::Collision::Update() {
    if (element_comp) {
        Composite* parent_comp = element_comp->parent_comp;
        auto i_begin = parent_comp->controls.begin();
        auto i_end = parent_comp->controls.end();
        for (auto i = i_begin; i != i_end; )
            if (*i == element_comp) {
                parent_comp->controls.erase(i);
                break;
            }
            else
                i++;

        element_comp->Free();
        element_comp = 0;
    }

    element_comp = new dw::Composite(group);

    const prj::vector<SkinParam::CollisionParam>* cls_list = rob_osage_test->get_cls_list();
    if (!cls_list)
        return;

    bone_database_skeleton* skeleton = bone_database_get_skeleton_by_type(BONE_DATABASE_SKELETON_COMMON);

    char buf[0x200];
    int32_t index = 0;
    for (const SkinParam::CollisionParam& i : *cls_list) {
        dw::Button* element_button = new dw::Button(element_comp, dw::RADIOBUTTON);
        switch (i.type) {
        case SkinParam::CollisionTypeEnd:
            sprintf_s(buf, sizeof(buf), "%d %s", index, collision_type_name_list[i.type]);
            break;
        case SkinParam::CollisionTypeBall:
        case SkinParam::CollisionTypeCapsule:
        case SkinParam::CollisionTypePlane:
        case SkinParam::CollisionTypeEllipse:
        case SkinParam::CollisionTypeAABB:
            sprintf_s(buf, sizeof(buf), "%d %s/%s", index,
                collision_type_name_list[i.type], skeleton->object_bone_names[i.node_idx[0]]);
            break;
        default:
            sprintf_s(buf, sizeof(buf), "%d", index);
            break;
        }

        element_button->SetText(buf);
        element_button->callback_data.i32 = index;
        element_button->callback = RobOsageTestDw::Collision::ElementCallback;
        if (index == rob_osage_test->collision_index)
            element_button->SetValue(true);

        if (i.type == SkinParam::CollisionTypeEnd)
            break;

        index++;
    }
}

void RobOsageTestDw::Collision::CopyCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button) {
        prj::vector<SkinParam::CollisionParam>* cls_list = rob_osage_test->get_cls_list();
        if (cls_list)
            rob_osage_test->cls_list_temp.assign(cls_list->begin(), cls_list->end());
    }
}

void RobOsageTestDw::Collision::ElementCallback(dw::Widget* data) {
    int32_t index = data->callback_data.i32;
    prj::vector<SkinParam::CollisionParam>* cls_list = rob_osage_test->get_cls_list();
    if (cls_list) {
        rob_osage_test->collision_index = index;

        rob_osage_test_dw->colli_element.Update();
    }
}

void RobOsageTestDw::Collision::PasteCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);

    if (button) {
        prj::vector<SkinParam::CollisionParam>* cls_list = rob_osage_test->get_cls_list();
        if (cls_list) {
            cls_list->assign(
                rob_osage_test->cls_list_temp.begin(), rob_osage_test->cls_list_temp.end());

            rob_osage_test->collision_index = -1;
            rob_osage_test->collision_update = true;
        }
    }
}

void RobOsageTestDw::Collision::ResetCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button) {
        prj::vector<SkinParam::CollisionParam>* cls_list = rob_osage_test->get_cls_list();
        if (cls_list) {
            cls_list->clear();
            cls_list->push_back({});
            rob_osage_test->collision_index = -1;
            rob_osage_test->collision_update = true;
        }
    }
}

void RobOsageTestDw::ColliElement::Init(dw::Composite* parent) {
    group = new dw::Group(parent);
    group->SetText("COLLI ELEMENT");

    dw::Composite* ribbon_comp = new dw::Composite(group);
    ribbon_comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    reset_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    reset_button->SetText("RESET ");
    reset_button->callback = RobOsageTestDw::ColliElement::ResetCallback;

    copy_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    copy_button->SetText(" COPY ");
    copy_button->callback = RobOsageTestDw::ColliElement::CopyCallback;

    paste_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    paste_button->SetText("PASTE ");
    paste_button->callback = RobOsageTestDw::ColliElement::PasteCallback;

    flip_button = new dw::Button(ribbon_comp, dw::FLAG_8);
    flip_button->SetText(" FLIP ");
    flip_button->callback = RobOsageTestDw::ColliElement::FlipCallback;

    type_list_box = new dw::ListBox(group, dw::MULTISELECT);
    for (const char*& i : collision_type_name_list)
        type_list_box->AddItem(i);
    type_list_box->AddSelectionListener(new dw::SelectionListenerOnHook(
        RobOsageTestDw::ColliElement::TypeCallback));

    radius_slider = dw::Slider::Create(group);
    radius_slider->SetText("R");
    radius_slider->format = "% 2.3f   ";
    radius_slider->SetParams(0.0f, 0.0f, 2.0f, 0.01f, 0.001f, 0.01f);
    radius_slider->AddSelectionListener(new dw::SelectionListenerOnHook(
        RobOsageTestDw::ColliElement::RadiusCallback));

    bone_database_skeleton* skeleton = bone_database_get_skeleton_by_type(BONE_DATABASE_SKELETON_COMMON);

    bone0_list_box = new dw::ListBox(group);
    bone0_list_box->SetMaxItems(20);
    for (uint32_t i = 0; i < skeleton->object_bone_name_count; i++)
        bone0_list_box->AddItem(skeleton->object_bone_names[i]);
    bone0_list_box->callback_data.i32 = 0;
    bone0_list_box->AddSelectionListener(new dw::SelectionListenerOnHook(
        RobOsageTestDw::ColliElement::BoneCallback));

    bone0_x_slider = dw::Slider::Create(group);
    bone0_x_slider->SetText("x");
    bone0_x_slider->format = "% 2.3f   ";
    bone0_x_slider->SetParams(0.0f, -1.0f, 1.0f, 0.1f, 0.01f, 0.1f);
    bone0_x_slider->callback_data.i32 = 0;
    bone0_x_slider->AddSelectionListener(new dw::SelectionListenerOnHook(
        RobOsageTestDw::ColliElement::BonePosXCallback));

    bone0_y_slider = dw::Slider::Create(group);
    bone0_y_slider->SetText("y");
    bone0_y_slider->format = "% 2.3f   ";
    bone0_y_slider->SetParams(0.0f, -1.0f, 1.0f, 0.1f, 0.01f, 0.1f);
    bone0_y_slider->callback_data.i32 = 0;
    bone0_y_slider->AddSelectionListener(new dw::SelectionListenerOnHook(
        RobOsageTestDw::ColliElement::BonePosYCallback));

    bone0_z_slider = dw::Slider::Create(group);
    bone0_z_slider->SetText("z");
    bone0_z_slider->format = "% 2.3f   ";
    bone0_z_slider->SetParams(0.0f, -1.0f, 1.0f, 0.1f, 0.01f, 0.1f);
    bone0_z_slider->callback_data.i32 = 0;
    bone0_z_slider->AddSelectionListener(new dw::SelectionListenerOnHook(
        RobOsageTestDw::ColliElement::BonePosZCallback));

    bone1_list_box = new dw::ListBox(group);
    bone1_list_box->SetMaxItems(20);
    for (uint32_t i = 0; i < skeleton->object_bone_name_count; i++)
        bone1_list_box->AddItem(skeleton->object_bone_names[i]);
    bone1_list_box->callback_data.i32 = 1;
    bone1_list_box->AddSelectionListener(new dw::SelectionListenerOnHook(
        RobOsageTestDw::ColliElement::BoneCallback));

    bone1_x_slider = dw::Slider::Create(group);
    bone1_x_slider->SetText("x");
    bone1_x_slider->format = "% 2.3f   ";
    bone1_x_slider->SetParams(0.0f, -1.0f, 1.0f, 0.1f, 0.01f, 0.1f);
    bone1_x_slider->callback_data.i32 = 1;
    bone1_x_slider->AddSelectionListener(new dw::SelectionListenerOnHook(
        RobOsageTestDw::ColliElement::BonePosXCallback));

    bone1_y_slider = dw::Slider::Create(group);
    bone1_y_slider->SetText("y");
    bone1_y_slider->format = "% 2.3f   ";
    bone1_y_slider->SetParams(0.0f, -1.0f, 1.0f, 0.1f, 0.01f, 0.1f);
    bone1_y_slider->callback_data.i32 = 1;
    bone1_y_slider->AddSelectionListener(new dw::SelectionListenerOnHook(
        RobOsageTestDw::ColliElement::BonePosYCallback));

    bone1_z_slider = dw::Slider::Create(group);
    bone1_z_slider->SetText("z");
    bone1_z_slider->format = "% 2.3f   ";
    bone1_z_slider->SetParams(0.0f, -1.0f, 1.0f, 0.1f, 0.01f, 0.1f);
    bone1_z_slider->callback_data.i32 = 1;
    bone1_z_slider->AddSelectionListener(new dw::SelectionListenerOnHook(
        RobOsageTestDw::ColliElement::BonePosZCallback));
}

void RobOsageTestDw::ColliElement::Update() {
    const SkinParam::CollisionParam* cls_param = rob_osage_test->get_cls_param();
    if (!cls_param)
        return;

    type_list_box->SetItemIndex(cls_param->type);

    radius_slider->SetValue(cls_param->radius);

    bone0_list_box->SetItemIndex(cls_param->node_idx[0]);

    bone0_x_slider->SetValue(cls_param->pos[0].x);
    bone0_y_slider->SetValue(cls_param->pos[0].y);
    bone0_z_slider->SetValue(cls_param->pos[0].z);

    bone1_list_box->SetItemIndex(cls_param->node_idx[1]);

    bone1_x_slider->SetValue(cls_param->pos[1].x);
    bone1_y_slider->SetValue(cls_param->pos[1].y);
    bone1_z_slider->SetValue(cls_param->pos[1].z);
}

void RobOsageTestDw::ColliElement::BoneCallback(dw::Widget* data) {
    dw::ListBox* list_box = dynamic_cast<dw::ListBox*>(data);
    if (list_box) {
        SkinParam::CollisionParam* cls_param = rob_osage_test->get_cls_param();
        if (cls_param) {
            cls_param->type = (SkinParam::CollisionType)(int32_t)rob_osage_test_dw->
                colli_element.type_list_box->list->selected_item;
            cls_param->node_idx[list_box->callback_data.i32] = (int32_t)list_box->list->selected_item;

            rob_osage_test->collision_update = true;
        }
    }
}

void RobOsageTestDw::ColliElement::BonePosXCallback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider) {
        SkinParam::CollisionParam* cls_param = rob_osage_test->get_cls_param();
        if (cls_param) {
            cls_param->type = (SkinParam::CollisionType)(int32_t)rob_osage_test_dw->
                colli_element.type_list_box->list->selected_item;
            cls_param->pos[slider->callback_data.i32].x = slider->scroll_bar->value;
        }
    }
}

void RobOsageTestDw::ColliElement::BonePosYCallback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider) {
        SkinParam::CollisionParam* cls_param = rob_osage_test->get_cls_param();
        if (cls_param) {
            cls_param->type = (SkinParam::CollisionType)(int32_t)rob_osage_test_dw->
                colli_element.type_list_box->list->selected_item;
            cls_param->pos[slider->callback_data.i32].y = slider->scroll_bar->value;
        }
    }
}

void RobOsageTestDw::ColliElement::BonePosZCallback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider) {
        SkinParam::CollisionParam* cls_param = rob_osage_test->get_cls_param();
        if (cls_param) {
            cls_param->type = (SkinParam::CollisionType)(int32_t)rob_osage_test_dw->
                colli_element.type_list_box->list->selected_item;
            cls_param->pos[slider->callback_data.i32].z = slider->scroll_bar->value;
        }
    }
}

void RobOsageTestDw::ColliElement::CopyCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button) {
        SkinParam::CollisionParam* cls_param = rob_osage_test->get_cls_param();
        if (cls_param)
            rob_osage_test->cls_param_temp = *cls_param;
    }
}

void RobOsageTestDw::ColliElement::FlipCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button) {
        SkinParam::CollisionParam* cls_param = rob_osage_test->get_cls_param();
        if (cls_param) {
            int32_t node_idx = cls_param->node_idx[0];
            vec3 pos = cls_param->pos[0];
            cls_param->node_idx[0] = cls_param->node_idx[1];
            cls_param->pos[0] = cls_param->pos[1];
            cls_param->node_idx[1] = node_idx;
            cls_param->pos[1] = pos;

            rob_osage_test->collision_update = true;
            rob_osage_test_dw->colli_element.Update();
        }
    }
}

void RobOsageTestDw::ColliElement::PasteCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button) {
        SkinParam::CollisionParam* cls_param = rob_osage_test->get_cls_param();
        if (cls_param) {
            *cls_param = rob_osage_test->cls_param_temp;

            rob_osage_test->collision_update = true;
            rob_osage_test_dw->colli_element.Update();
        }
    }
}

void RobOsageTestDw::ColliElement::RadiusCallback(dw::Widget* data) {
    dw::Slider* slider = dynamic_cast<dw::Slider*>(data);
    if (slider) {
        SkinParam::CollisionParam* cls_param = rob_osage_test->get_cls_param();
        if (cls_param) {
            cls_param->type = (SkinParam::CollisionType)(int32_t)rob_osage_test_dw->
                colli_element.type_list_box->list->selected_item;
            cls_param->radius = slider->scroll_bar->value;
        }
    }
}

void RobOsageTestDw::ColliElement::ResetCallback(dw::Widget* data) {
    dw::Button* button = dynamic_cast<dw::Button*>(data);
    if (button) {
        SkinParam::CollisionParam* cls_param = rob_osage_test->get_cls_param();
        if (cls_param) {
            *cls_param = {};

            rob_osage_test->collision_update = true;
            rob_osage_test_dw->colli_element.Update();
        }
    }
}

void RobOsageTestDw::ColliElement::TypeCallback(dw::Widget* data) {
    dw::ListBox* list_box = dynamic_cast<dw::ListBox*>(data);
    if (list_box) {
        SkinParam::CollisionParam* cls_param = rob_osage_test->get_cls_param();
        if (cls_param) {
            cls_param->type = (SkinParam::CollisionType)(int32_t)list_box->list->selected_item;

            rob_osage_test->collision_update = true;
        }
    }
}

RobOsageTestDw::RobOsageTestDw() : rob(), flags(), root(), node(), boc(), collision(), colli_element() {
    SetText("RobOsage TEST");

    dw::Composite* main_comp = new dw::Composite(this);
    main_comp->SetLayout(new dw::RowLayout(dw::HORIZONTAL));

    dw::Composite* left_comp = new dw::Composite(main_comp);

    rob.Init(left_comp);
    flags.Init(left_comp);
    root.Init(left_comp);
    node.Init(left_comp);
    boc.Init(left_comp);

    dw::Composite* right_comp = new dw::Composite(main_comp);

    collision.Init(right_comp);
    colli_element.Init(right_comp);

    UpdateLayout();

    rect.pos.x = (float_t)res_window_get()->width - rect.size.x;
    rect.pos.y = 160.0f;

    SetSize(rect.size);
}

RobOsageTestDw::~RobOsageTestDw() {

}

void RobOsageTestDw::Hide() {
    SetDisp();
}

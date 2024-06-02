/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/prj/vector_pair.hpp"
#include "../rob/rob.hpp"
#include "../task.hpp"
#include "../types.hpp"

class RobOsageTest : public app::Task {
public:
    struct Node {
        struct Hinge {
            float_t ymin;
            float_t ymax;
            float_t zmin;
            float_t zmax;

            inline Hinge() {
                ymin = -90.0f;
                ymax = 90.0f;
                zmin = -90.0f;
                zmax = 90.0f;
            }
        };

        float_t coli_r;
        float_t weight;
        float_t inertial_cancel;
        Hinge hinge;

        inline Node() {
            coli_r = 0.0f;
            weight = 1.0f;
            inertial_cancel = 0.0f;
        }
    };

    struct Root {
        float_t force;
        float_t gain;
        float_t air_res;
        float_t root_y_rot;
        float_t root_z_rot;
        float_t fric;
        float_t wind_afc;
        SkinParam::RootCollisionType coli_type;
        float_t init_y_rot;
        float_t init_z_rot;

        inline Root() {
            force = 0.0f;
            gain = 0.0f;
            air_res = 0.5f;
            root_y_rot = 0.0f;
            root_z_rot = 0.0f;
            fric = 1.0f;
            wind_afc = 0.5f;
            coli_type = SkinParam::RootCollisionTypeEnd;
            init_y_rot = 0.0f;
            init_z_rot = 0.0f;
        }
    };

    bool load;
    bool save;
    bool coli;
    bool line;

    int32_t chara_id;
    int32_t load_chara_id;
    ::item_id item_id;
    object_info obj_info;
    size_t osage_index;
    size_t collision_index;

    prj::vector_pair_alloc<::item_id, object_info> objects;
    prj::vector_pair_alloc<ExOsageBlock*, prj::vector<skin_param_osage_root_boc>> bocs;
    prj::vector_pair_alloc<ExOsageBlock*, prj::vector<skin_param_osage_node>> nodes;
    prj::vector_pair_alloc<ExOsageBlock*, prj::vector<skin_param_osage_root_normal_ref>> normal_refs;

    Root root;
    Node node;

    Root root_temp;
    Node node_temp;
    prj::vector<SkinParam::CollisionParam> cls_list_temp;
    SkinParam::CollisionParam cls_param_temp;

    bool collision_update;

    RobOsageTest();
    virtual ~RobOsageTest() override;

    virtual bool init() override;
    virtual bool ctrl() override;
    virtual bool dest() override;
    virtual void disp() override;
    virtual void basic() override;

    void disp_coli();
    void disp_line();

    ExClothBlock* get_cloth_block(rob_chara_item_equip_object* itm_eq_obj) const;
    rob_chara_item_equip_object* get_item_equip_object() const;
    ExOsageBlock* get_osage_block(rob_chara_item_equip_object* itm_eq_obj) const;
    prj::vector<SkinParam::CollisionParam>* get_cls_list(ExClothBlock* cls) const;
    prj::vector<SkinParam::CollisionParam>* get_cls_list(ExOsageBlock* osg) const;
    SkinParam::CollisionParam* get_cls_param(prj::vector<SkinParam::CollisionParam>* cls_list) const;
    void set_root(skin_param* skp);

    inline prj::vector<SkinParam::CollisionParam>* get_cls_list() const {
        rob_chara_item_equip_object* itm_eq_obj = get_item_equip_object();
        ExOsageBlock* osg = get_osage_block(itm_eq_obj);
        return osg ? get_cls_list(osg) : get_cls_list(get_cloth_block(itm_eq_obj));
    }

    inline SkinParam::CollisionParam* get_cls_param() const {
        return get_cls_param(get_cls_list());
    }

    inline skin_param* get_skin_param() const {
        rob_chara_item_equip_object* itm_eq_obj = get_item_equip_object();
        ExOsageBlock* osg = get_osage_block(itm_eq_obj);
        if (osg)
            return osg->rob.skin_param_ptr;
        else {
            ExClothBlock* cls = get_cloth_block(itm_eq_obj);
            if (cls)
                return cls->rob.skin_param_ptr;
        }
        return 0;
    }

    inline void set_root() {
        return set_root(get_skin_param());
    }
};

extern RobOsageTest* rob_osage_test;

extern void rob_osage_test_init();
extern void rob_osage_test_free();

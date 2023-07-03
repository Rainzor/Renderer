#include "box.h"

box::box(const pointf3 &p0, const pointf3 &p1, shared_ptr<material> m) {
    box_min = p0;
    box_max = p1;

    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), m));
    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), m));

    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), m));
    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), m));

    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), m));
    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), m));
    mat_ptr = m;
}


bool box::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = aabb(box_min, box_max);
    return true;
}

bool box::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    return sides.hit(r, t_min, t_max, rec);
}

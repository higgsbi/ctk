#include "ctk/math/aabb.h"
#include "ctk/io.h"

int main() {
    AABB box = aabb(&vec3(-16, -16, -16), &vec3(16, 16, 16));

    assert(aabb_contains(&box, &vec3(0, 0, 0)));
    print(&str("Box contains (0,0,0)\n"));

    assert(aabb_contains(&box, &vec3(16, 16, 16)));
    print(&str("Box contains (16,16,16)\n"));

    assert(aabb_contains(&box, &vec3(-16, -16, -16)));
    print(&str("Box contains (-16,-16,-16)\n"));

    assert(aabb_contains(&box, &vec3(16, 0, -16)));
    print(&str("Box contains (16,0,-16)\n"));

    assert(!aabb_contains(&box, &vec3(0, 0, -17)));
    print(&str("Box does not contains (0,0,-17)\n"));

    AABB intersecting = aabb(&vec3(-20, -20, -20), &vec3(10, 10, 10));
    AABB separate = aabb(&vec3(-20, -20, -20), &vec3(-40, -9, -10000));
    AABB encompassing = aabb_expand(&separate, &vec3(10000, 10000, 10000));
    assert(aabb_intersects(&box, &intersecting));
    assert(!aabb_intersects(&box, &separate));
    assert(aabb_intersects(&box, &encompassing));
    print(&str("Boxes intersect properly\n"));

    return 0;
}

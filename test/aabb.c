#include "ctk/math/aabb.h"
#include "ctk/io.h"
#include "ctk/types/string.h"

int main()
{
    AABB box = aabb(&vec3(-16, -16, -16), &vec3(16, 16, 16));

    assert(aabb_contains(&box, &vec3(0, 0, 0)));
    print(&str("Box contains %v\n"), &vec3(0, 0, 0));

    assert(aabb_contains(&box, &vec3(16, 16, 16)));
    print(&str("Box contains %v\n"), &vec3(16, 16, 16));

    assert(aabb_contains(&box, &vec3(-16, -16, -16)));
    print(&str("Box contains %v\n"), &vec3(-16, -16, -16));

    assert(aabb_contains(&box, &vec3(16, 0, -16)));
    print(&str("Box contains %v\n"), &vec3(16, 0, -16));

    assert(!aabb_contains(&box, &vec3(0, 0, -17)));
    print(&str("Box does not contains %v\n"), &vec3(0, 0, -17));

    AABB intersecting = aabb(&vec3(-20, -20, -20), &vec3(10, 10, 10));
    AABB separate = aabb(&vec3(-20, -20, -20), &vec3(-40, -9, -10000));
    AABB encompassing = aabb_expand(&separate, &vec3(10000, 10000, 10000));
    assert(aabb_intersects(&box, &intersecting));
    assert(!aabb_intersects(&box, &separate));
    assert(aabb_intersects(&box, &encompassing));
    print(&str("Boxes intersect properly\n"));

    print(&str("\nAABB 1: %B\nAABB 2: %B\nAABB 3: %B\n"), &intersecting, &encompassing, &separate);

    return 0;
}

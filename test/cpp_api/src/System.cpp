#include <cpp_api.h>

void System_action() {
    flecs::world world;

    flecs::component<Position>(world, "Position");
    flecs::component<Velocity>(world, "Velocity");

    auto entity = flecs::entity(world)
        .set<Position>({10, 20})
        .set<Velocity>({1, 2});

    flecs::system<Position, Velocity>(world)
        .action([](flecs::iter&it, flecs::column<Position> p, flecs::column<Velocity> v) {
            for (auto i : it) {
                p[i].x += v[i].x;
                p[i].y += v[i].y;
            }
        });

    world.progress();

    const Position *p = entity.get<Position>();
    test_int(p->x, 11);
    test_int(p->y, 22);

    const Velocity *v = entity.get<Velocity>();
    test_int(v->x, 1);
    test_int(v->y, 2);       
}

void System_action_const() {
    flecs::world world;

    flecs::component<Position>(world, "Position");
    flecs::component<Velocity>(world, "Velocity");

    auto entity = flecs::entity(world)
        .set<Position>({10, 20})
        .set<Velocity>({1, 2});

    flecs::system<Position, const Velocity>(world)
        .action([](flecs::iter&it, flecs::column<Position> p, flecs::column<const Velocity> v) {
            for (auto i : it) {
                p[i].x += v[i].x;
                p[i].y += v[i].y;
            }
        });

    world.progress();

    const Position *p = entity.get<Position>();
    test_int(p->x, 11);
    test_int(p->y, 22);

    const Velocity *v = entity.get<Velocity>();
    test_int(v->x, 1);
    test_int(v->y, 2);
}

void System_action_shared() {
    flecs::world world;

    flecs::component<Position>(world, "Position");
    flecs::component<Velocity>(world, "Velocity");

    auto base = flecs::entity(world)
        .set<Velocity>({1, 2});

    auto e1 = flecs::entity(world)
        .set<Position>({10, 20})
        .add_instanceof(base);

    auto e2 = flecs::entity(world)
        .set<Position>({10, 20})
        .set<Velocity>({3, 4});

    flecs::system<Position>(world).signature("ANY:Velocity")
        .action([](flecs::iter&it, flecs::column<Position> p) {
            flecs::column<const Velocity> v(it, 2);

            if (v.is_shared()) {
                for (auto i : it) {
                    p[i].x += v->x;
                    p[i].y += v->y;
                }
            } else {
                for (auto i : it) {
                    p[i].x += v[i].x;
                    p[i].y += v[i].y;
                }                
            }
        });

    world.progress();

    const Position *p = e1.get<Position>();
    test_int(p->x, 11);
    test_int(p->y, 22);

    p = e2.get<Position>();
    test_int(p->x, 13);
    test_int(p->y, 24);   
}

void System_each() {
    flecs::world world;

    flecs::component<Position>(world, "Position");
    flecs::component<Velocity>(world, "Velocity");

    auto entity = flecs::entity(world)
        .set<Position>({10, 20})
        .set<Velocity>({1, 2});

    flecs::system<Position, Velocity>(world)
        .each([](flecs::entity e, Position& p, Velocity& v) {
            p.x += v.x;
            p.y += v.y;
        });

    world.progress();

    const Position *p = entity.get<Position>();
    test_int(p->x, 11);
    test_int(p->y, 22);
}

void System_each_const() {
    flecs::world world;

    flecs::component<Position>(world, "Position");
    flecs::component<Velocity>(world, "Velocity");

    auto entity = flecs::entity(world)
        .set<Position>({10, 20})
        .set<Velocity>({1, 2});

    flecs::system<Position, const Velocity>(world)
        .each([](flecs::entity e, Position& p, const Velocity& v) {
            p.x += v.x;
            p.y += v.y;
        });

    world.progress();

    const Position *p = entity.get<Position>();
    test_int(p->x, 11);
    test_int(p->y, 22);
}

void System_each_shared() {
    flecs::world world;

    flecs::component<Position>(world, "Position");
    flecs::component<Velocity>(world, "Velocity");

    auto base = flecs::entity(world)
        .set<Velocity>({1, 2});

    auto e1 = flecs::entity(world)
        .set<Position>({10, 20})
        .add_instanceof(base);

    auto e2 = flecs::entity(world)
        .set<Position>({10, 20})
        .set<Velocity>({3, 4});

    flecs::system<Position, const Velocity>(world)
        .each([](flecs::entity e, Position& p, const Velocity& v) {
            p.x += v.x;
            p.y += v.y;
        });

    world.progress();

    const Position *p = e1.get<Position>();
    test_int(p->x, 11);
    test_int(p->y, 22);

    p = e2.get<Position>();
    test_int(p->x, 13);
    test_int(p->y, 24); 
}

void System_signature() {
    flecs::world world;

    flecs::component<Position>(world, "Position");
    flecs::component<Velocity>(world, "Velocity");

    auto entity = flecs::entity(world)
        .set<Position>({10, 20})
        .set<Velocity>({1, 2});

    flecs::system<>(world).signature("Position, Velocity")
        .action([](flecs::iter&it) {
            flecs::column<Position> p(it, 1);
            flecs::column<Velocity> v(it, 2);

            for (auto i : it) {
                p[i].x += v[i].x;
                p[i].y += v[i].y;
            }
        });

    world.progress();

    const Position *p = entity.get<Position>();
    test_int(p->x, 11);
    test_int(p->y, 22);

    const Velocity *v = entity.get<Velocity>();
    test_int(v->x, 1);
    test_int(v->y, 2); 
}

void System_signature_const() {
    flecs::world world;

    flecs::component<Position>(world, "Position");
    flecs::component<Velocity>(world, "Velocity");

    auto entity = flecs::entity(world)
        .set<Position>({10, 20})
        .set<Velocity>({1, 2});

    flecs::system<>(world).signature("Position, [in] Velocity")
        .action([](flecs::iter&it) {
            flecs::column<Position> p(it, 1);
            flecs::column<const Velocity> v(it, 2);

            for (auto i : it) {
                p[i].x += v[i].x;
                p[i].y += v[i].y;
            }
        });

    world.progress();

    const Position *p = entity.get<Position>();
    test_int(p->x, 11);
    test_int(p->y, 22);

    const Velocity *v = entity.get<Velocity>();
    test_int(v->x, 1);
    test_int(v->y, 2); 
}

void System_signature_shared() {
    flecs::world world;

    flecs::component<Position>(world, "Position");
    flecs::component<Velocity>(world, "Velocity");

    auto base = flecs::entity(world)
        .set<Velocity>({1, 2});

    auto e1 = flecs::entity(world)
        .set<Position>({10, 20})
        .add_instanceof(base);

    auto e2 = flecs::entity(world)
        .set<Position>({10, 20})
        .set<Velocity>({3, 4});

    flecs::system<>(world).signature("ANY:Position, [in] ANY:Velocity")
        .action([](flecs::iter&it) {
            flecs::column<Position> p(it, 1);
            flecs::column<const Velocity> v(it, 2);

            if (v.is_shared()) {
                for (auto i : it) {
                    p[i].x += v->x;
                    p[i].y += v->y;
                }
            } else {
                for (auto i : it) {
                    p[i].x += v[i].x;
                    p[i].y += v[i].y;
                }                
            }
        });

    world.progress();

    const Position *p = e1.get<Position>();
    test_int(p->x, 11);
    test_int(p->y, 22);

    p = e2.get<Position>();
    test_int(p->x, 13);
    test_int(p->y, 24); 
}

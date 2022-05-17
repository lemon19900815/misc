#pragma once

#include <cstdint>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "../stdafx.h"

namespace maze {

enum Direction {
    North,
    South,
    East,
    West,
};

class MapSite {
public:
    virtual ~MapSite() {}
    virtual void Enter() = 0;

    CLASS_PTR(MapSite);
};

class Room : public MapSite {
public:
    Room(int num) {
        room_no_ = num;
        for (auto& side : sides_) {
            side = nullptr;
        }
    }
    Room(const Room& other) {
        room_no_ = other.room_no_;
        for (int32_t i = 0; i < 4; ++i) {
            sides_[i] = other.sides_[i];
        }
    }
    ~Room() {
    }

    MapSite::Ptr GetSide(Direction dir) {
        return sides_[dir];
    }

    void SetSide(Direction dir, MapSite::Ptr map_site) {
        sides_[dir] = map_site;
    }

    void Enter() override {
        std::cout << "enter room: " << room_no_ << std::endl;
    }

    int32_t room_no() {
        return room_no_;
    }

    CLASS_PTR(Room);

    virtual Room::Ptr Clone() {
        return std::make_shared<Room>(*this);
    }

    virtual void Initilize(int32_t no) {
        room_no_ = no;
    }

private:
    MapSite::Ptr sides_[4];
    int32_t room_no_;
};

class Wall : public MapSite {
public:
    Wall() {
    }
    Wall(const Wall& other) {
    }
    ~Wall() {
    }

    void Enter() override {
    }

    CLASS_PTR(Wall);

    virtual Wall::Ptr Clone() {
        return std::make_shared<Wall>(*this);
    }
};

class Door : public MapSite {
public:
    Door(Room::Ptr r1, Room::Ptr r2) {
        is_open_ = false;
        room1_ = r1;
        room2_ = r2;
    }
    Door(const Door& other) {
        is_open_ = other.is_open_;
        room1_ = other.room1_;
        room2_ = other.room2_;
    }
    ~Door() {
    }

    void Enter() override {

    }
    Room::Ptr OtherSideFrom(Room::Ptr room) {
        return room == room1_ ? room2_ : room1_;
    }

    CLASS_PTR(Door);

    virtual Door::Ptr Clone() {
        return std::make_shared<Door>(*this);
    }
    virtual void Initialize(Room::Ptr r1, Room::Ptr r2) {
        room1_ = r1;
        room2_ = r2;
    }

private:
    Room::Ptr room1_;
    Room::Ptr room2_;
    bool is_open_;
};

class Maze {
public:
    Maze() {
    }
    Maze(const Maze& other) {
        maze_rooms_ = other.maze_rooms_;
    }
    ~Maze() {
    }

    void AddRoom(Room::Ptr r) {
        maze_rooms_[r->room_no()] = r;
    }

    Room::Ptr GetRoom(int room_no) {
        auto itr = maze_rooms_.find(room_no);
        return itr != maze_rooms_.end() ? itr->second : nullptr;
    }

    CLASS_PTR(Maze);

    Maze::Ptr Clone() {
        return std::make_shared<Maze>(*this);
    }

private:
    std::unordered_map<int32_t, Room::Ptr> maze_rooms_;
};

class MazeFactory {
public:
    virtual ~MazeFactory() {
    }

    virtual Maze::Ptr MakeMaze() {
        return std::make_shared<Maze>();
    }

    virtual Wall::Ptr MakeWall() {
        return std::make_shared<Wall>();
    }
    virtual Room::Ptr MakeRoom(int num) {
        return std::make_shared<Room>(num);
    }
    virtual Door::Ptr MakeDoor(Room::Ptr r1, Room::Ptr r2) {
        return std::make_shared<Door>(r1, r2);
    }
};

class Spell {
public:
    CLASS_PTR(Spell);
};

// Ӧ��ʹ��װ��ģʽȥ����
class EnchantedRoom : public Room {
public:
    EnchantedRoom(int num, Spell::Ptr spell) : Room(num) {
        spell_ = spell;
    }

private:
    Spell::Ptr spell_;
};

// Ӧ��ʹ��װ��ģʽȥ����
class DoorNeedingSpell : public Door {
public:
    DoorNeedingSpell(Room::Ptr r1, Room::Ptr r2) : Door(r1, r2) {
    }
};

class EnchantedMazeFactory : public MazeFactory {
public:
    Room::Ptr MakeRoom(int num) override {
        return std::make_shared<EnchantedRoom>(num, CastSpell());
    }

    Door::Ptr MakeDoor(Room::Ptr r1, Room::Ptr r2) override {
        return std::make_shared<DoorNeedingSpell>(r1, r2);
    }

private:
    Spell::Ptr CastSpell() {
        return std::make_shared<Spell>();
    }
};

class BombedWall : public Wall {
public:
};

class RoomWithABomb : public Room {
public:
    RoomWithABomb(int num) : Room(num) {
    }
};

class BombedMazeFactory : public MazeFactory {
public:
    Wall::Ptr MakeWall() override {
        return std::make_shared<BombedWall>();
    }

    Room::Ptr MakeRoom(int num) override {
        return std::make_shared<RoomWithABomb>(num);
    }
};

// ������/������ģʽ
class MazeBuilder {
public:
    virtual void BuildMaze() {}
    virtual void BuildRoom(int32_t num) {}
    virtual void BuildDoor(int32_t room_from, int32_t room_to) {}
    virtual Maze::Ptr GetMaze() { return nullptr; }

protected:
    MazeBuilder() {
    }
};

class StandardMazeBuilder : public MazeBuilder {
public:
    StandardMazeBuilder() {
        cur_maze_ = nullptr;
    }

    void BuildMaze() override {
        if (!cur_maze_) {
            cur_maze_ = std::make_shared<Maze>();
        }
    }

    void BuildRoom(int32_t num) override {
        if (cur_maze_ && !cur_maze_->GetRoom(num)) {
            Room::Ptr room = std::make_shared<Room>(num);
            cur_maze_->AddRoom(room);

            room->SetSide(North, std::make_shared<Wall>());
            room->SetSide(South, std::make_shared<Wall>());
            room->SetSide(East, std::make_shared<Wall>());
            room->SetSide(West, std::make_shared<Wall>());
        }
    }

    void BuildDoor(int32_t room_from, int32_t room_to) override {
        if (cur_maze_) {
            auto r1 = cur_maze_->GetRoom(room_from);
            auto r2 = cur_maze_->GetRoom(room_to);
            auto door = std::make_shared<Door>(r1, r2);

            r1->SetSide(CommonWall(r1, r2), door);
            r2->SetSide(CommonWall(r2, r1), door);
        }
        
    }

    Maze::Ptr GetMaze() override {
        return cur_maze_;
    }

private:
    Direction CommonWall(Room::Ptr r1, Room::Ptr r2) {
        if (r1->GetSide(East) == r2->GetSide(West))
            return East;
        if (r1->GetSide(South) == r2->GetSide(North))
            return South;
        if (r1->GetSide(West) == r2->GetSide(East))
            return West;
        //if (r1->GetSide(North) == r2->GetSide(South))
            return North;
    }

private:
    Maze::Ptr cur_maze_;
};

class CountingMazeBuilder : public MazeBuilder {
public:
    CountingMazeBuilder() {
        rooms_ = 0;
        doors_ = 0;
    }

    void BuildMaze() override {
    }
    void BuildRoom(int32_t num) override {
        rooms_ += 1;
    }
    void BuildDoor(int32_t room_from, int32_t room_to) override {
        doors_ += 1;
    }

    void GetCounts(int32_t& rooms, int32_t& doors) {
        rooms = rooms_;
        doors = doors_;
    }

private:
    int32_t rooms_;
    int32_t doors_;
};

class MazeGame {
public:
    Maze::Ptr CreateMaze() {
        auto maze = std::make_shared<Maze>();

        auto r1 = std::make_shared<Room>(1);
        auto r2 = std::make_shared<Room>(2);
        auto door = std::make_shared<Door>(r1, r2);

        maze->AddRoom(r1);
        maze->AddRoom(r2);

        r1->SetSide(North, std::make_shared<Wall>());
        r1->SetSide(East, door);
        r1->SetSide(South, std::make_shared<Wall>());
        r1->SetSide(West, std::make_shared<Wall>());

        r2->SetSide(North, std::make_shared<Wall>());
        r2->SetSide(East, std::make_shared<Wall>());
        r2->SetSide(South, std::make_shared<Wall>());
        r2->SetSide(West, door);

        return maze;
    }

    Maze::Ptr CreateMaze(MazeFactory& factory) {
        auto maze = factory.MakeMaze();

        auto r1 = factory.MakeRoom(1);
        auto r2 = factory.MakeRoom(2);
        auto door = factory.MakeDoor(r1, r2);

        maze->AddRoom(r1);
        maze->AddRoom(r2);

        r1->SetSide(North, factory.MakeWall());
        r1->SetSide(East, door);
        r1->SetSide(South, factory.MakeWall());
        r1->SetSide(West, factory.MakeWall());

        r2->SetSide(North, factory.MakeWall());
        r2->SetSide(East, factory.MakeWall());
        r2->SetSide(South, factory.MakeWall());
        r2->SetSide(West, door);

        return maze;
    }

    Maze::Ptr CreateMaze(MazeBuilder& builder) {
        builder.BuildMaze();

        builder.BuildRoom(1);
        builder.BuildRoom(2);
        builder.BuildDoor(1, 2);

        return builder.GetMaze();
    }
};

class MazeGameV2 {
public:
    Maze::Ptr CreateMaze() {
        auto maze = MakeMaze();

        auto r1 = MakeRoom(1);
        auto r2 = MakeRoom(2);

        maze->AddRoom(r1);
        maze->AddRoom(r2);

        auto door = MakeDoor(r1, r2);

        r1->SetSide(North, MakeWall());
        r1->SetSide(East, door);
        r1->SetSide(South, MakeWall());
        r1->SetSide(West, MakeWall());

        r2->SetSide(North, MakeWall());
        r2->SetSide(East, MakeWall());
        r2->SetSide(South, MakeWall());
        r2->SetSide(West, door);

        return maze;
    }

    // factory method:
    virtual Maze::Ptr MakeMaze() {
        return std::make_shared<Maze>();
    }
    virtual Room::Ptr MakeRoom(int32_t n) {
        return std::make_shared<Room>(n);
    }
    virtual Wall::Ptr MakeWall() {
        return std::make_shared<Wall>();
    }
    virtual Door::Ptr MakeDoor(Room::Ptr r1, Room::Ptr r2) {
        return std::make_shared<Door>(r1, r2);
    }
};

class BombedMazeGame : public MazeGameV2 {
public:
    Wall::Ptr MakeWall() override {
        return std::make_shared<BombedWall>();
    }
    Room::Ptr MakeRoom(int32_t n) override {
        return std::make_shared<RoomWithABomb>(n);
    }
};

void factoryTest() {
    MazeGame game;

    // Ӳ���봴��maze��������Ӧ�µ���Ϸ�淨��
    // ������ħ������bomb�ĵ��������ݣ���Ҫ�޸Ĵ���maze�Ĺ���
    auto maze1 = game.CreateMaze();

    // ���󹤳�������ͨ��maze
    MazeFactory factory;
    auto maze2 = game.CreateMaze(factory);

    // ���󹤳�������spell��maze
    EnchantedMazeFactory enchanted_factory;
    auto maze3 = game.CreateMaze(enchanted_factory);

    // ���󹤳�������bomb��maze
    BombedMazeFactory bombed_factory;
    auto maze4 = game.CreateMaze(bombed_factory);

    // ���ģʽ���ǲ������޸Ĵ��룬���ǰѾ����仯���ǲ��ֽ��з�װ������
    // ���ﾭ���仯�Ŀ��ܾ������Ե��淨����maze�Ĵ�������һ�㲻�ᷢ���仯��
    // ����仯��ͨ������Ϊ��ά�ִ���Ĳ��䡣

    /*
        ���󹤳�ģʽ���ŵ㣺
        1�������˾������
        2��ʹ�����ڽ�����Ʒϵ�У���ħ�����Թ����ߴ�ը�����Թ��ȣ�
        3�������ڲ�Ʒ��һ����

        ȱ�㣺
        1������֧��������Ĳ�Ʒ��������Ʒ��Ҫ�޸�Factory������������ࣩ

        ���ģʽ��
        1��AbstractFactory���󹤳���ͨ���ù���������Factory Method��ʵ�֣�������Ҳ������Prototypeʵ�֡�
        2��һ������Ĺ���ͨ����һ��������Singleton����
    */
}

void builderTest() {
    MazeGame game;
    StandardMazeBuilder builder;

    auto maze1 = game.CreateMaze(builder);

    CountingMazeBuilder counting_builder;
    auto maze2 = game.CreateMaze(counting_builder);

    int32_t rooms, doors;
    counting_builder.GetCounts(rooms, doors);

    std::cout << "The maze has "
        << rooms << " rooms and "
        << doors << " doors" << std::endl;

    // builder��factory�Ƚ�
    // 1��builder������һ��������һ�����ӵĶ���
}

class InviteCode {
public:
    virtual ~InviteCode() {}

    bool Exchange() {
        if (!CheckCode())
            return false;
        if (!CheckValid())
            return false;
        if (!GiveReward())
            return false;
        Record();
        return true;
    }

    CLASS_PTR(InviteCode);

protected:
    virtual bool CheckCode() {
        return true;
    }

    virtual bool CheckValid() {
        return true;
    }

    virtual bool GiveReward() {
        return true;
    }

    virtual void Record() {
    }
};

class ErrorCode : public InviteCode {
protected:
    bool CheckCode() override {
        return false;
    }
};

class OnceCode : public InviteCode {
protected:
    bool CheckValid() override {
        // TODO���������Ƿ��Ѿ���ȡ��ͬ���͵�������
        return true;
    }

    void Record() override {
        // TODO����¼��ȡ�������͵������룬�Ժ���������ȡ������
    }
};

class ActivityCode : public InviteCode {
protected:
    bool CheckValid() override {
        // TODO���������Ƿ��Ѿ���ȡ��ͬ���͵�������
        return true;
    }

    void Record() override {
        // TODO����¼��ȡ�������͵������룬�Ժ���������ȡ������
    }
};

class InviteCodeFactory {
public:
    template<class TYPE>
    static InviteCode::Ptr Create() {
        return std::make_shared<TYPE>();
    }
};

void factoryMethodTest() {
    MazeGameV2 game;
    auto maze1 = game.CreateMaze();

    BombedMazeGame game2;
    auto maze2 = game2.CreateMaze();

    // ����ʾ����������ʹ��
    auto invite_code = InviteCodeFactory::Create<OnceCode>();
    invite_code->Exchange();

    invite_code = InviteCodeFactory::Create<ActivityCode>();
    invite_code->Exchange();
}

class MazePrototypeFactory : public MazeFactory {
public:
    MazePrototypeFactory(Maze::Ptr prototype_maze,
        Wall::Ptr prototype_wall, Room::Ptr prototype_room,
        Door::Ptr prototype_door) {
        prototype_maze_ = prototype_maze;
        prototype_wall_ = prototype_wall;
        prototype_room_ = prototype_room;
        prototype_door_ = prototype_door;
    }

    Maze::Ptr MakeMaze() override {
        return prototype_maze_->Clone();
    }
    Wall::Ptr MakeWall() override {
        return prototype_wall_->Clone();
    }
    Room::Ptr MakeRoom(int num) override {
        auto room = prototype_room_->Clone();
        room->Initilize(num);
        return room;
    }
    Door::Ptr MakeDoor(Room::Ptr r1, Room::Ptr r2) override {
        auto door = prototype_door_->Clone();
        door->Initialize(r1, r2);
        return door;
    }

private:
    Maze::Ptr prototype_maze_;
    Wall::Ptr prototype_wall_;
    Room::Ptr prototype_room_;
    Door::Ptr prototype_door_;
};

void prototypeTest() {
    MazeGame game;

    auto prototype_maze = std::make_shared<Maze>();
    auto prototype_wall = std::make_shared<Wall>();
    auto prototype_room = std::make_shared<Room>(0);
    auto prototype_door = std::make_shared<Door>(
        std::make_shared<Room>(1), std::make_shared<Room>(2));

    MazePrototypeFactory simpleMazeFactory(
        prototype_maze,
        prototype_wall,
        prototype_room,
        prototype_door);

    auto simple_maze = game.CreateMaze(simpleMazeFactory);

    auto bombed_wall = std::make_shared<BombedWall>();
    MazePrototypeFactory bombedMazeFactory(
        prototype_maze,
        bombed_wall,
        prototype_room,
        prototype_door);

    auto bombed_maze = game.CreateMaze(bombedMazeFactory);

    /* 
        ԭ��ģʽ�ͳ��󹤳��ıȽϣ�
        1��ԭ�Ͳ���Ҫ�ٴ�������ʵ������BombedFactory&EnchantedFactory�ȣ�
        2��ʹ�ó��󹤳�ʱ������ҪΪÿһ��Product����ʵ��Clone�������ر���Clone��������ʱ
    */
}

void test() {
    factoryTest();
    builderTest();
    factoryMethodTest();
    prototypeTest();
}

} // namespace maze

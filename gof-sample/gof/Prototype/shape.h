#ifndef __Prototype_Shape_Inc_H__
#define __Prototype_Shape_Inc_H__

#include "../stdafx.h"
#include <memory>
#include <unordered_map>

namespace Prototype
{
	class Shape : public std::enable_shared_from_this<Shape>
	{
	public:
		typedef std::shared_ptr<Shape> PTR;

	public:
		virtual ~Shape() {

		}

		Shape::PTR clone() {
			return shared_from_this();
		}

		const std::string type() {
			return type_;
		}

		virtual void draw() = 0;

	protected:
		std::string type_;
	};

	class Circle : public Shape
	{
	public:
		Circle() {
			type_ = "circle";
		}
		~Circle() override {

		}

		void draw() override {
			std::cout << "Circle: " << type_ << " draw." << std::endl;
		}
	};

	class Square : public Shape
	{
	public:
		Square() {
			type_ = "square";
		}
		~Square() override {

		}

		void draw() override {
			std::cout << "Square: " << type_ << " draw." << std::endl;
		}
	};

	class Rectangle : public Shape
	{
	public:
		Rectangle() {
			type_ = "rectangle";
		}
		~Rectangle() override {

		}

		void draw() override {
			std::cout << "Rectangle: " << type_ << " draw." << std::endl;
		}
	};

	class ShapeCache
	{
	public:
		static void loadCache() {
			auto shape1 = std::make_shared<Circle>();
			shapes_[shape1->type()] = shape1;

			auto shape2 = std::make_shared<Square>();
			shapes_[shape2->type()] = shape2;

			auto shape3 = std::make_shared<Rectangle>();
			shapes_[shape3->type()] = shape3;
		}

		static void unloadCache() {
			shapes_.clear();
		}

		// prototype is not useful for c++, we can use c++11 shared_ptr to manage the reference
		static Shape::PTR getShape(const std::string& type) {
			auto it = shapes_.find(type);
			return it != shapes_.end() ? it->second->clone() : nullptr;
		}

	private:
		static std::unordered_map<std::string, Shape::PTR> shapes_;
	};

	std::unordered_map<std::string, Shape::PTR> ShapeCache::shapes_;

	namespace PrototypePatternDemo
	{
		void test()
		{
			std::cout << "\n\n prototype pattern." << std::endl;

			ShapeCache::loadCache();

			auto shape1 = ShapeCache::getShape("circle");
			shape1->draw();

			auto shape2 = ShapeCache::getShape("square");
			shape2->draw();

			auto shape3 = ShapeCache::getShape("rectangle");
			shape3->draw();

			ShapeCache::unloadCache();
		}
	}
}

#endif // __Prototype_Shape_Inc_H__

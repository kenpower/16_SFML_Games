#include "pch.h"
#include "../16_SFML_Games/EntityController.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(Collision, SameEntityControllerCollidesWithItself) {
	
	EntityController *e = new EntityController;
	e->settings(0, 0);
	
	EXPECT_TRUE(isCollide(e, e));
}

TEST(Collision, OvelappingEntityControllersCollide) {

	EntityController* e = new EntityController;
	e->settings(0, 0);
	EntityController* f = new EntityController;
	f->settings(0.5, 0);

	EXPECT_TRUE(isCollide(e, f));	
	EXPECT_TRUE(isCollide(f, e));
}

TEST(Collision, SeperatedEntityControllersDoNotCollide) {

	EntityController* e = new EntityController;
	e->settings(0, 0);
	EntityController* f = new EntityController;
	f->settings(2.5, 0);

	EXPECT_FALSE(isCollide(e, f));	
	EXPECT_FALSE(isCollide(f, e));
}

TEST(Collision, EntityControllerInsideAnotherCollide) {

	EntityController* e = new EntityController;
	e->settings(0, 0, 0, 100);
	EntityController* f = new EntityController;
	f->settings(2.5, 0);

	EXPECT_TRUE(isCollide(e, f));
	EXPECT_TRUE(isCollide(f, e));
}
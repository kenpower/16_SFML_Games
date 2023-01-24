#include "pch.h"
#include "../16_SFML_Games/EntityController.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(Collision, SameEntityControllerCollidesWithIteslf) {
	
	EntityController *e = new EntityController;
	e->settings(0, 0);
	
	EXPECT_TRUE(isCollide(e, e));
}
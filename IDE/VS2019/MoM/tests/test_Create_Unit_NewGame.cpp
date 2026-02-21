#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#include "../src/UNITTYPE.h"
#ifdef __cplusplus
}
#endif

#include <cstring>

class Create_Unit_NewGame_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        memset(_UNITS, 0, sizeof(_UNITS));
        memset(_CITIES, 0, sizeof(_CITIES));
        memset(_players, 0, sizeof(_players));
        _units = 0;
    }
};

TEST_F(Create_Unit_NewGame_test, ZeroUnitType_DoesNotCreateUnit)
{
    // Act
    Create_Unit_NewGame(0, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_units, 0) << "Should not create unit with type 0";
}

TEST_F(Create_Unit_NewGame_test, ValidUnitType_CreatesUnit)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_units, 1) << "Should create one unit";
    EXPECT_EQ(_UNITS[0].type, ut_BarbSpearmen);
    EXPECT_EQ(_UNITS[0].wx, 30);
    EXPECT_EQ(_UNITS[0].wy, 20);
    EXPECT_EQ(_UNITS[0].wp, ARCANUS_PLANE);
    EXPECT_EQ(_UNITS[0].owner_idx, 0);
}

TEST_F(Create_Unit_NewGame_test, SetsUnitNotFinished)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].Finished, ST_FALSE) << "New units should not be finished";
}

TEST_F(Create_Unit_NewGame_test, SetsMovesToMax)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].moves2, _UNITS[0].moves2_max) << "Moves should equal max moves";
}

TEST_F(Create_Unit_NewGame_test, InitializesDamageToZero)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].Damage, 0);
}

TEST_F(Create_Unit_NewGame_test, InitializesXPToZero)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].XP, 0);
    EXPECT_EQ(_UNITS[0].Level, 0);
}

TEST_F(Create_Unit_NewGame_test, InitializesEnchantmentsToZero)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].enchantments, 0);
}

TEST_F(Create_Unit_NewGame_test, SetsStatusToReady)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].Status, us_Ready);
}

TEST_F(Create_Unit_NewGame_test, SetsHeroSlotToUndefined)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].Hero_Slot, ST_UNDEFINED);
}

TEST_F(Create_Unit_NewGame_test, AlchemyPlayer_SetsMagicWeaponryFlag)
{
    // Arrange
    _players[0].alchemy = ST_TRUE;

    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_NE(_UNITS[0].mutations & wq_Magic, 0) << "Should have magic weaponry with Alchemy";
}

TEST_F(Create_Unit_NewGame_test, NoAlchemy_NoMagicWeaponryFlag)
{
    // Arrange
    _players[0].alchemy = ST_FALSE;

    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].mutations, 0) << "Should not have magic weaponry without Alchemy";
}

TEST_F(Create_Unit_NewGame_test, MultipleUnits_IncrementsCounter)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);
    Create_Unit_NewGame(ut_BarbSwordsmen, 0, 31, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_units, 2) << "Should create two units";
    EXPECT_EQ(_UNITS[0].type, ut_BarbSpearmen);
    EXPECT_EQ(_UNITS[1].type, ut_BarbSwordsmen);
}

TEST_F(Create_Unit_NewGame_test, MyrrorPlane_SetsCorrectPlane)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, MYRROR_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].wp, MYRROR_PLANE);
}

TEST_F(Create_Unit_NewGame_test, DifferentPlayers_SetCorrectOwner)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);
    Create_Unit_NewGame(ut_BarbSpearmen, 1, 31, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].owner_idx, 0);
    EXPECT_EQ(_UNITS[1].owner_idx, 1);
}

TEST_F(Create_Unit_NewGame_test, SetsRdConstrLeftToUndefined)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].Rd_Constr_Left, ST_UNDEFINED);
}

TEST_F(Create_Unit_NewGame_test, SetsMoveFailedToFalse)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].Move_Failed, ST_FALSE);
}

TEST_F(Create_Unit_NewGame_test, ClearsDstCoordinates)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].dst_wx, 0);
    EXPECT_EQ(_UNITS[0].dst_wy, 0);
}

TEST_F(Create_Unit_NewGame_test, SetsDrawPriorityToZero)
{
    // Act
    Create_Unit_NewGame(ut_BarbSpearmen, 0, 30, 20, ARCANUS_PLANE, -1);

    // Assert
    EXPECT_EQ(_UNITS[0].Draw_Priority, 0);
}

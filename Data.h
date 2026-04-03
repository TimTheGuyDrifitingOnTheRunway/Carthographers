#pragma once
//Ce document est là pour réunir tous les contenus comme les pièces ou les Saisons

#include "Structs.h"

/******************************** LibCartographie.h ********************************/
// Notaion : L_XXX = petit XXX ; B_XXX = GRAND XXX

extern const Piece POINT;
extern const Piece NOTHING;

extern const Piece U;

extern const Piece L;
extern const Piece L_L;
extern const Piece B_L;

extern const Piece LINE;
extern const Piece L_LINE;

extern const Piece T;
extern const Piece L_T;

extern const Piece CUBE;
extern const Piece CUBE_WITH_POINT;
extern const Piece RECT;
extern const Piece RECT_WITH_HOLE;

extern const Piece DIAG;
extern const Piece L_DIAG;

extern const Piece CROSS;
extern const Piece STAIRS;
extern const Piece STRANGE;

extern const Piece Z;
extern const Piece B_Z;


/******************************** Lib3d.h ********************************/

// Cartes Saison

extern const Saison Spring;
extern const Saison Summer;
extern const Saison Autumn;
extern const Saison Winter;

extern const Saison* seasons[4];


// Cartes Scoring

extern const ScoringCard SentinelWood;
extern const ScoringCard TreeTower;
extern const ScoringCard GreenBough;
extern const ScoringCard StoneSideQuest;

extern const ScoringCard CanalLake;
extern const ScoringCard MagesValley;
extern const ScoringCard GoldenGranary;
extern const ScoringCard ShoreSideExpanse;

extern const ScoringCard Wildholds;
extern const ScoringCard GreengoldPlains;
extern const ScoringCard GreatCity;
extern const ScoringCard Shieldgate;

extern const ScoringCard Borderlands;
extern const ScoringCard BrokenRoad;
extern const ScoringCard LostBarony;
extern const ScoringCard TheCauldrons;

extern const ScoringCard* scoringCards[16];


// Cartes Exploration

extern const ExploreCard Farmland;
extern const ExploreCard ForgottenForest;
extern const ExploreCard Hamlet;
extern const ExploreCard GreatRiver;
extern const ExploreCard HinterlandStream;
extern const ExploreCard Homestead;
extern const ExploreCard Orchard;
extern const ExploreCard Marshlands;
extern const ExploreCard TreetopVillage;
extern const ExploreCard FishingVillage;


// Cartes Ennemis

extern const ExploreCard BugbearAssault;
extern const ExploreCard GoblinAttack;
extern const ExploreCard FlayerIncursion;
extern const ExploreCard GnollRaid;
extern const ExploreCard InsectoidInvasion;
extern const ExploreCard OgreCharge;
extern const ExploreCard RatmanStrike;
extern const ExploreCard KoboldOnlaught;


// Ruines et RiftLands

extern const ExploreCard OutpostRuins;
extern const ExploreCard TempleRuins;
extern const ExploreCard RiftLands;

extern const ExploreCard* expCards[21];

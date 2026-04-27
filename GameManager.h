#pragma once

#include "Data.h"
#include "LibCarthographie.h"
#include "Lib3d.h"
#include "UI.h"


// Setup
void SetupGame(GameState* gs);
void InitDeck(GameState* gs);
void ShakeDeck(GameState* gs);
void InitScoringCards(GameState* gs);
void EndGame(GameState* gs, int nbPlayers);


void StartGame(GameState* gs, Camera3D camera);
// Saison 
void NextSeason(GameState* gs, Camera3D camera, ModelList models, Seed s2, int mountainSeed[2]);
void Season(GameState* gs, Camera3D camera, ModelList models);//première saison
void Season2(GameState* gs, Camera3D camera, ModelList models, Seed s, int mountainSeed[2]);// 2ème saison et autre


// Tour de jeu

const ExploreCard* Turn(GameState* gs, int* isRuin, Camera3D *camera, int mountainSeed[2], ModelList models, Seed s);
const ExploreCard* NextExploreCard(GameState* gs, int* isRuin);

void DebugGameStats(GameState* gs);

/******************Prototypes******************/
int CalcPointsFromCards(FeuilleCarte f, ScoringCard* cards, int numberOfCards);





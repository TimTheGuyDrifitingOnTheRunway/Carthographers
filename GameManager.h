#pragma once

#include "Data.h"
#include "LibCarthographie.h"
#include "Lib3d.h"


// Setup
void SetupGame(GameState* gs);
void InitDeck(GameState* gs);
void ShakeDeck(GameState* gs);
void InitScoringCards(GameState* gs);
void EndGame(GameState* gs, int nbPlayers);


void StartGame(GameState* gs, Camera3D camera);
// Saison
void NextSeason(GameState* gs, Camera3D camera);
void Season(GameState* gs, Camera3D camera);


// Tour de jeu

const ExploreCard* Turn(GameState* gs, int* index, int* isRuin, Camera3D camera);
const ExploreCard* NextExploreCard(GameState* gs, int* index, int* isRuin);

void DebugGameStats(GameState* gs);

/******************Prototypes******************/
int CalcPointsFromCards(FeuilleCarte f, ScoringCard* cards, int numberOfCards);





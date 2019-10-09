
#include "game_state.h"


static GameState gameState;
static GameState track = gameState;
static int selectedStage = 0;
static unsigned int frameCounter = 0;
static int gameClearOrMiss = 0;
static int toEnding = 0;
static int player1DeadCount = 0;
static int player2DeadCount = 0;
static int enemyKillCount = 0;
static int towerDamage = 0;
static int towerMaxHp = 0;

GameState getGameState() {
	return gameState;
}
void initGameState() {
	frameCounter = 0;
}
void changeGameState(GameState next) {
	track = next;
	gameState = MOVING;
}
bool isChangingGameState() {
	return gameState == MOVING;
}
void moveGameState() {
	gameState = track;
}
void setSelectedStage(int stage){
	selectedStage = stage - 1;
}
void updateGameFrame() {
	frameCounter++;
}
void intoGameStart(){
	//‚¢‚ë‚¢‚ëƒŠƒZƒbƒg‚·‚é
	gameClearOrMiss = 0;
	toEnding = 0;
	player1DeadCount = 0;
	player2DeadCount = 0;
	enemyKillCount = 0;
	towerDamage = 0;
	towerMaxHp = 0;
}
void intoGameClear(){
	gameClearOrMiss = 1;
}
void intoEnding(){
	toEnding = 1;
}
void intoGameOver(){
	gameClearOrMiss = -1;
}

int getSelectedStage(){
	return selectedStage;
}
unsigned int getGameFrame() {
	return frameCounter;
}
int isGameClear(){
	if(gameClearOrMiss <= 0){
		return 0;
	}
	return gameClearOrMiss;
}
int isGameEnding(){
	return toEnding;
}
int isGameOver(){
	if(gameClearOrMiss >= 0){
		return 0;
	}
	return gameClearOrMiss;
}

int countUpPlayer1Dead(){
	player1DeadCount++;
	return 0;
}
int countUpPlayer2Dead(){
	player2DeadCount++;
	return 0;
}
int countUpEnemyKill(){
	enemyKillCount++;
	return 0;
}
int addTowerDamege(int damage){
	towerDamage += damage;
	return 0;
}
int setTowerMaxHp(int hp){
	towerMaxHp = hp;
	return 0;
}

int getPlayer1DownCount(){
	return player1DeadCount;
}
int getPlayer2DownCount(){
	return player2DeadCount;
}
int getEnemyKillCount(){
	return enemyKillCount;
}
double getTopwerDurability(){
	double t = 0;
	int towerHp = towerMaxHp - towerDamage;
	t = (double)towerHp / towerMaxHp;
	return t;
}

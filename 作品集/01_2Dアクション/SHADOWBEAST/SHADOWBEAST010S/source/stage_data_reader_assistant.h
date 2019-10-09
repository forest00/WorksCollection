#pragma once

int SetImage(int index, int handle);

int loadImageByData(int index, char *filePathAndName);

int dispImageInGame(int index);

int clearImageInGame(int index);

int moveImageInGame(int index, int x, int y);


int initStageDataReaderAssistant();

int updateStageDataReaderAssistant();

int drawStageDataReaderAssistant();


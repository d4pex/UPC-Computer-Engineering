#include "TxTancaSessio.h"
#include "PetitFlix.h"

void TxTancaSessio::executar() {
	PetitFlix& pt = PetitFlix::getInstance();
	pt.tancaSessio();
}
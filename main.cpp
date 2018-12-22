/*
    Developed by Regson Jaques
    https://www.youtube.com/user/tictacKode/playlists
    https://github.com/tictackode
    https://www.facebook.com/groups/ProgDesafios/
*/

#include "tetris_functions.h"

int main()
{
    init();

	initBlockColor();

	loadAll();

	LOCK_FUNCTION(incrementaTimer);
	LOCK_VARIABLE(timer);

	timer = 0;

    zeraTela();      // preenche com '0' a matriz tela
    Moldura();      // desenha a borda vermelha do jogo
    srand ( time(0) );


	//setBackground();

	animation();

    loopPrincipal();

	liberaMemoria();
    deinit();
	return 0;

}
END_OF_MAIN()

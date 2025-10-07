#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "player.cc"
#include "parcheesi.cc"


int main() {
    Parcheesi parchis{4};  

    srand(time(NULL));

    bool win = false;

    printf("=== INICIO DEL JUEGO DE PARCHIS ===\n\n");

    do {
        printf("Turno del jugador %d (%d repeticiones de seis)\n",
               parchis.current_player_, parchis.repeated_sixes_);

        // Tirar el dado
        int dice = RollDice();
        printf("El jugador %d ha sacado un %d\n", parchis.current_player_, dice);

        // Elegir ficha (en este ejemplo elegimos la primera que pueda moverse)
        int selected_piece = -1;
        for (int i = 0; i < Player::kPieceNum; i++) {
            if (parchis.player_[parchis.current_player_].piece_[i].position != -1) {
                selected_piece = i;
                break;
            }
        }

        if (selected_piece == -1)
            selected_piece = 0;

        // Aplicar movimiento según las reglas
        MovementOutput result = parchis.ApplyMovement(selected_piece, dice);

        // Mostrar resultado textual del movimiento
        switch (result) {
            case kMovementOutput_BasicMove:
                printf("Movimiento básico completado.\n");
                break;
            case kMovementOutput_NoMove:
                printf("No se ha podido mover la ficha.\n");
                break;
            case kMovementOutput_Eat:
                printf("¡Ha comido una ficha rival y avanza 20 casillas!\n");
                break;
            case kMovementOutput_IllegalMove:
                printf("¡Tres seises seguidos! La ficha vuelve a casa.\n");
                break;
            case kMovementOutput_End:
                printf("El jugador ha terminado la partida.\n");
                win = true;
                break;
            default:
                break;
        }

        // Comprobar si todas las fichas de un jugador llegaron al final
        int finished = 0;
        for (int i = 0; i < Player::kPieceNum; i++) {
            if (parchis.player_[parchis.current_player_].piece_[i].position >= Parcheesi::finishPoint[parchis.current_player_]) {
                finished++;
            }
        }
        if (finished == Player::kPieceNum) {
            printf("¡Jugador %d ha ganado la partida!\n", parchis.current_player_);
            win = true;
        }

        printf("\n");
    } while (!win);

    printf("=== FIN DEL JUEGO ===\n");
    return 0;
}


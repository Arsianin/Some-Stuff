// Jen mě nebijte
// MAKEFILE je rekomendace, že??
// Rozumně bych ten kód vysvětloval osobně, jelikož jsem s nim tak pracoval
// Dokonce top programátoři často mají nečitelný kód

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// #include "heart_im_pirate.png"
//  Deklarace globálních paramentů
#define STOLB 10
#define RYAD 5

#define SHIROTAEKRANA 1800
#define VYSOTAEKRANA 900

// #define HVEZDY 30

int main()
{
    // Deklarace parametrů SDL
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("SDL experiments", 0, 0, SHIROTAEKRANA, VYSOTAEKRANA, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // bool vklucenie_psvedo_taimera = false;
    // Deklarace všech proměnnych a structur
    // Nazvy je hrůza)
    int scet_zivych = RYAD * STOLB;
    int total_scet_pstime = 0;
    SDL_Event e;
    bool quit = false;
    int start_stolb = 0; // update
    int polozenie_pushki1 = SHIROTAEKRANA / 2;

    bool shot_igroka = false;
    bool shot_aliens = true;
    int scet_zhizney_igroka = 3;

    bool popadanie_detected = true;
    int podbityi_objekt_x[RYAD];
    int podbityi_objekt_y[STOLB];
    int scet_bodbitych = 0;
    int proverka_stroki_dlya_vystrela = 4;
    int random_vystrel_vragov = 0;

    int sqre_polo_x = 0;
    int sqre_polo_y = 0;
    int polozhenie_otobrazhenia_zhizney = 10;
    // int popadanie_po_zemle = 0;
    bool change_side = false;
    SDL_Rect vybuch = {.x = 0, .y = VYSOTAEKRANA - 75, .w = 5, .h = 10};
    SDL_Rect lives[3];
    for (int lives_scet_for = 0; lives_scet_for < 3; lives_scet_for++)
    {
        lives[lives_scet_for].x = polozhenie_otobrazhenia_zhizney;
        lives[lives_scet_for].y = VYSOTAEKRANA - 150;
        lives[lives_scet_for].w = 25;
        lives[lives_scet_for].h = 25;
        polozhenie_otobrazhenia_zhizney += 40;
    }

    SDL_Rect snaryad = {.x = polozenie_pushki1, .y = 800, .w = 2, .h = 10};

    SDL_Rect pushka_koroblya = {.x = polozenie_pushki1, .y = 800, .w = 4, .h = 10};
    SDL_Rect korpus_koroblya = {.x = polozenie_pushki1 - 22, .y = 810, .w = 50, .h = 10};

    // Výzdoba obrazovky
    SDL_Rect morya_okeany = {.x = 0, .y = VYSOTAEKRANA - 75, .w = SHIROTAEKRANA, .h = 75};
    SDL_Rect lesa1 = {.x = 200, .y = VYSOTAEKRANA - 75, .w = 200, .h = 40};
    SDL_Rect lesa2 = {.x = 400, .y = VYSOTAEKRANA - 75, .w = 150, .h = 75};
    SDL_Rect pustynya1 = {.x = 700, .y = VYSOTAEKRANA - 75, .w = 200, .h = 75};
    SDL_Rect pustynya2 = {.x = 900, .y = VYSOTAEKRANA - 75, .w = 200, .h = 50};
    SDL_Rect sev_polyus = {.x = SHIROTAEKRANA - 150, .y = VYSOTAEKRANA - 75, .w = 150, .h = 75};
    SDL_Rect sev_polyus_os = {.x = SHIROTAEKRANA - 250, .y = VYSOTAEKRANA - 60, .w = 20, .h = 30};
    SDL_Rect ostrov = {.x = 1200, .y = VYSOTAEKRANA - 40, .w = 75, .h = 30};
    SDL_Rect stars[30];
    int star_poloz_x = 50;
    int star_poloz_y = 50;
    for (int j = 0; j < 30; j++)
    {
        if (j % 6 == 0 && j != 0)
        {
            stars[j].y = star_poloz_y + (75 + rand() % 150);
            star_poloz_x = 50;
        }
        else
        {
            stars[j].y = star_poloz_y;
        }
        stars[j].x = star_poloz_x + (100 + rand() % 200);

        stars[j].w = 2;
        stars[j].h = 2;
        star_poloz_x = stars[j].x;
        star_poloz_y = stars[j].y;
    }
    SDL_Rect r[RYAD][STOLB];
    for (int j = 0; j < RYAD; j++)
    {
        for (int i = 0; i < STOLB; i++)
        {

            r[j][i].x = sqre_polo_x;
            r[j][i].y = sqre_polo_y;
            r[j][i].w = 50;
            r[j][i].h = 25;
            sqre_polo_x += 75;
        }
        sqre_polo_y += 50;
        sqre_polo_x -= 75 * STOLB;
    }
    SDL_Rect okno_alien[RYAD][STOLB];
    SDL_Rect vrazheskiy_snaryad = {.x = r[3][5].x, .y = r[3][5].y, .w = 2, .h = 10};

    //-----TIME-----//
    srand(time(NULL));
    // GAME IS ON
    while (!quit)
    {
        // řízení
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.key.keysym.sym == SDLK_SPACE && shot_igroka != true)
            {
                shot_igroka = true;

                snaryad.x = polozenie_pushki1;
            }
            if (e.key.keysym.sym == SDLK_RIGHT)
            {
                polozenie_pushki1 += 10;
                pushka_koroblya.x += 10;
                korpus_koroblya.x += 10;
            }
            else if (e.key.keysym.sym == SDLK_LEFT)
            {
                polozenie_pushki1 -= 10;
                pushka_koroblya.x -= 10;
                korpus_koroblya.x -= 10;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Nastavení barvy na černou //Classic
        SDL_RenderClear(renderer);                      // Vykreslení pozadí

        // Hvězdy
        for (int i = 0; i < 30; i++)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &stars[i]);
            SDL_RenderDrawRect(renderer, &stars[i]);
        }
        // Země
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        SDL_RenderFillRect(renderer, &morya_okeany);
        SDL_RenderDrawRect(renderer, &morya_okeany);
        SDL_SetRenderDrawColor(renderer, 50, 150, 50, 255);
        SDL_RenderFillRect(renderer, &lesa1);
        SDL_RenderDrawRect(renderer, &lesa1);
        SDL_SetRenderDrawColor(renderer, 50, 130, 50, 255);
        SDL_RenderFillRect(renderer, &lesa2);
        SDL_RenderDrawRect(renderer, &lesa2);
        SDL_SetRenderDrawColor(renderer, 150, 150, 0, 255);
        SDL_RenderFillRect(renderer, &pustynya1);
        SDL_RenderDrawRect(renderer, &pustynya1);
        SDL_SetRenderDrawColor(renderer, 175, 150, 0, 255);
        SDL_RenderFillRect(renderer, &pustynya2);
        SDL_RenderDrawRect(renderer, &pustynya2);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &sev_polyus);
        SDL_RenderDrawRect(renderer, &sev_polyus);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);
        SDL_RenderFillRect(renderer, &sev_polyus_os);
        SDL_RenderDrawRect(renderer, &sev_polyus_os);
        SDL_SetRenderDrawColor(renderer, 50, 130, 50, 255);
        SDL_RenderFillRect(renderer, &ostrov);
        SDL_RenderDrawRect(renderer, &ostrov);
        // hráč SHOT check
        if (shot_igroka == true)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Nastavení barvy na červenou

            SDL_RenderDrawRect(renderer, &snaryad);
            snaryad.y -= 5;
            // Vykreslení čáry
            //(render, 1 bod, 2 bod, x, y);
        }
        // alien SHOT check
        if (shot_aliens == true)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Nastavení barvy na červenou

            SDL_RenderDrawRect(renderer, &vrazheskiy_snaryad);
            vrazheskiy_snaryad.y += 5;
            // Vykreslení čáry
            //(render, 1 bod, 2 bod, x, y);
        }
        int prosto_desyat = 0;
        for (int klop = 0; klop < STOLB; klop++)
        {
            if (r[proverka_stroki_dlya_vystrela][klop].h == 0)
            {
                prosto_desyat += 1;
            }
        }
        if (prosto_desyat == 10)
        {
            proverka_stroki_dlya_vystrela -= 1;
        }
        // CHeck střet
        if (vrazheskiy_snaryad.y == VYSOTAEKRANA - 75)
        {
            vybuch.x = vrazheskiy_snaryad.x;
            SDL_SetRenderDrawColor(renderer, 0, 200, 30, 255);
            SDL_RenderDrawRect(renderer, &vybuch);
            int pustoi_stolb = 0;
            int rvv_pam = random_vystrel_vragov;
            random_vystrel_vragov = rand() % 10;

            ///----------!UPDATE!----------vvv
            //---------check mt sloupec---//

            for (int plok = 0; plok < RYAD; plok++)
            {
                if (r[plok][random_vystrel_vragov].h == 0)
                {
                    pustoi_stolb += 1;
                }
            }
            if (pustoi_stolb == 5)
            {
                random_vystrel_vragov = start_stolb;
            }
            else
            {
                start_stolb = random_vystrel_vragov;
            }
            ///-------------------------------^^^
            shot_aliens = false;
            if (r[proverka_stroki_dlya_vystrela][random_vystrel_vragov].h != 0)
            {
                vrazheskiy_snaryad.x = r[proverka_stroki_dlya_vystrela][random_vystrel_vragov].x;
                vrazheskiy_snaryad.y = r[proverka_stroki_dlya_vystrela][random_vystrel_vragov].y;
            }
            else
            {
                vrazheskiy_snaryad.x = r[proverka_stroki_dlya_vystrela][rvv_pam].x;
                vrazheskiy_snaryad.y = r[proverka_stroki_dlya_vystrela][rvv_pam].y;
            }

            shot_aliens = true;
        }

        //-----střet mimozemšťanského vystřelu a hráče------//
        if (SDL_HasIntersection(&korpus_koroblya, &vrazheskiy_snaryad)) //---//---//--//
        {

            int rvv_pam = random_vystrel_vragov;
            random_vystrel_vragov = rand() % 10;
            shot_aliens = false;
            if (r[proverka_stroki_dlya_vystrela][random_vystrel_vragov].h != 0)
            {
                vrazheskiy_snaryad.x = r[proverka_stroki_dlya_vystrela][random_vystrel_vragov].x;
                vrazheskiy_snaryad.y = r[proverka_stroki_dlya_vystrela][random_vystrel_vragov].y;
            }
            else
            {
                vrazheskiy_snaryad.x = r[proverka_stroki_dlya_vystrela][rvv_pam].x;
                vrazheskiy_snaryad.y = r[proverka_stroki_dlya_vystrela][rvv_pam].y;
            }
            shot_aliens = true;
            scet_zhizney_igroka -= 1;
        }

        if (scet_zhizney_igroka == 0)
        {
            printf("Game over\n"); // LOSS
            quit = true;
        }

        SDL_SetRenderDrawColor(renderer, 75, 100, 75, 100);
        SDL_RenderFillRect(renderer, &pushka_koroblya);
        SDL_RenderDrawRect(renderer, &pushka_koroblya);

        SDL_SetRenderDrawColor(renderer, 75, 100, 75, 100);
        SDL_RenderFillRect(renderer, &korpus_koroblya);
        SDL_RenderDrawRect(renderer, &korpus_koroblya);

        //-----střet vystřelu hráče a mimozemšťana------//

        for (int j = 0; j < RYAD; j++)
        {
            for (int i = 0; i < STOLB; i++)
            {
                if (SDL_HasIntersection(&r[j][i], &snaryad))
                {
                    shot_igroka = false;

                    snaryad.y = 800;

                    r[j][i].w = 0;
                    r[j][i].h = 0;
                    scet_zivych -= 1;
                }
            }
        }

        if (snaryad.y <= 5)
        {
            shot_igroka = false;

            snaryad.y = 800;
        }

        for (int j = 0; j < RYAD; j++)
        {
            for (int i = 0; i < STOLB; i++)
            {
                if (r[j][i].h == 0 && r[j][i].w == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &r[j][i]);
                }
                else
                {
                    if (total_scet_pstime > 250 && total_scet_pstime < 350)
                    {
                        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 50);
                        SDL_RenderFillRect(renderer, &r[j][i]);
                        SDL_RenderDrawRect(renderer, &r[j][i]);
                    }
                    else
                    {
                        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 50);
                        SDL_RenderFillRect(renderer, &r[j][i]);
                        SDL_RenderDrawRect(renderer, &r[j][i]);
                    }
                }
            }
        }

        //---------------------------------------------------------------------------------------------v
        // Pohyb mimozemšťanů
        if (r[0][0].x <= 0)
        {
            change_side = false;
            for (int j = 0; j < RYAD; j++)
            {
                for (int i = 0; i < STOLB; i++)
                {
                    r[j][i].y += 5;
                }
            }
        }
        if (r[0][STOLB - 1].x >= SHIROTAEKRANA - 50)
        {
            change_side = true;
            for (int j = 0; j < RYAD; j++)
            {
                for (int i = 0; i < STOLB; i++)
                {
                    r[j][i].y += 5;
                }
            }
        }

        //----------------------------------------------------------------------------------------------x
        if (change_side == false && (total_scet_pstime % 30) == 0)
        {
            for (int j = 0; j < RYAD; j++)
            {
                for (int i = 0; i < STOLB; i++)
                {
                    r[j][i].x += 20;
                }
            }
        }
        if (change_side == true && (total_scet_pstime % 30) == 0)
        {
            for (int j = 0; j < RYAD; j++)
            {
                for (int i = 0; i < STOLB; i++)
                {
                    r[j][i].x -= 20;
                }
            }
        }

        for (int j = 0; j < RYAD; j++)
        {
            for (int i = 0; i < STOLB; i++)
            {
                if (r[j][i].y == VYSOTAEKRANA - 100 && r[j][i].h != 0)
                {
                    quit = true;
                    printf("Game over\n"); // LOSS
                    break;                 // update
                }
            }
        }

        //----------------------------------------------------------------------------------------------^
        // Životy hráča
        for (int kap = 0; kap < scet_zhizney_igroka; kap++)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &lives[kap]);
            SDL_RenderDrawRect(renderer, &lives[kap]);
        }

        total_scet_pstime += 1;

        if (total_scet_pstime == 600)
        {
            total_scet_pstime = 0;
        }
        //-----WIN------//
        if (scet_zivych == 0)
        {
            printf("Victory!\n");
            quit = true;
        }
        SDL_RenderPresent(renderer); // Prezentace kreslítka
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

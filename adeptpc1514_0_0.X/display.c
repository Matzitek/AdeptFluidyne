/* #############################################################################
 * EMFM main board firmware for Adept Fluidyne
 *
 * Module: display.c
 * Version 1.0
 *
 * Author:  Chris de Villiers
 *          Matzitek Innovative Technology (Pty) Ltd.
 * July 2015
 *
 * NOTES:
 *
 * 1.
 *
 * 2.
 *
 * #############################################################################
 */


#include "p24F32KA304.h"
#include "main.h"
#include "display.h"
#include "menu.h"

const unsigned char epd_data[8] = {0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xE0, 0x00};
//const char top_menu_run_str[3] = {"Run"};

unsigned char scan, temp; //, i;
unsigned char scan_data[24];
unsigned char display_line_old[75];
unsigned char display_line_new[75];
unsigned char display_row[14][50];
unsigned char cog_data;

const unsigned char char_zero[14][3] = {{0xAB, 0xFE, 0xAA},
{0xAF, 0xFF, 0xAA}, {0xBF, 0xAF, 0xEA}, {0xBE, 0xAB, 0xEA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFB, 0xFE, 0xFA},
{0xFB, 0xFE, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xBE, 0xAB, 0xEA}, {0xBF, 0xAF, 0xEA}, {0xAF, 0xFF, 0xAA},
{0xAB, 0xFE, 0xAA}};

const unsigned char char_one[14][3] = {{0xAA, 0xFA, 0xAA},
{0xAF, 0xFA, 0xAA}, {0xAF, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAF, 0xFF, 0xAA},
{0xAF, 0xFF, 0xAA}};

const unsigned char char_two[14][3] = {{0xAB, 0xFE, 0xAA},
{0xBF, 0xFF, 0xEA}, {0xFE, 0xAB, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xAA, 0xAA, 0xFA}, {0xAA, 0xAB, 0xFA}, {0xAA, 0xAF, 0xEA},
{0xAA, 0xFF, 0xAA}, {0xAF, 0xFA, 0xAA}, {0xBF, 0xAA, 0xAA},
{0xFE, 0xAA, 0xAA}, {0xFA, 0xAA, 0xAA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}};

const unsigned char char_three[14][3] = {{0xAB, 0xFE, 0xAA},
{0xBF, 0xFF, 0xEA}, {0xFE, 0xAB, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xAA, 0xAA, 0xFA}, {0xAA, 0xAB, 0xFA}, {0xAA, 0xAF, 0xEA},
{0xAA, 0xAF, 0xEA}, {0xAA, 0xAB, 0xFA}, {0xAA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFE, 0xAB, 0xFA}, {0xBF, 0xFF, 0xEA},
{0xAB, 0xFE, 0xAA}};

const unsigned char char_four[14][3] = {{0xAA, 0xAF, 0xAA},
{0xAA, 0xBF, 0xAA}, {0xAA, 0xFF, 0xAA}, {0xAB, 0xEF, 0xAA},
{0xAF, 0xAF, 0xAA}, {0xBE, 0xAF, 0xAA}, {0xFA, 0xAF, 0xAA},
{0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA}, {0xAA, 0xAF, 0xAA},
{0xAA, 0xAF, 0xAA}, {0xAA, 0xAF, 0xAA}, {0xAA, 0xAF, 0xAA},
{0xAA, 0xAF, 0xAA}};

const unsigned char char_five[14][3] = {{0xFF, 0xFF, 0xEA},
{0xFF, 0xFF, 0xEA}, {0xFA, 0xAA, 0xAA}, {0xFA, 0xAA, 0xAA},
{0xFA, 0xAA, 0xAA}, {0xFF, 0xFF, 0xAA}, {0xFF, 0xFF, 0xEA},
{0xAA, 0xAB, 0xFA}, {0xAA, 0xAA, 0xFA}, {0xAA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFE, 0xAB, 0xFA}, {0xBF, 0xFF, 0xEA},
{0xAB, 0xFE, 0xAA}};

const unsigned char char_six[14][3] = {{0xAB, 0xFE, 0xAA},
{0xBF, 0xFF, 0xEA}, {0xFE, 0xAB, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xAA}, {0xFB, 0xFE, 0xAA}, {0xFF, 0xFF, 0xEA},
{0xFE, 0xAB, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFE, 0xAB, 0xFA}, {0xBF, 0xFF, 0xEA},
{0xAB, 0xFE, 0xAA}};

const unsigned char char_seven[14][3] = {{0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}, {0xAA, 0xAA, 0xFA}, {0xAA, 0xAB, 0xEA},
{0xAA, 0xAB, 0xEA}, {0xAA, 0xAF, 0xAA}, {0xAA, 0xAF, 0xAA},
{0xAA, 0xBE, 0xAA}, {0xAA, 0xBE, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAB, 0xEA, 0xAA}, {0xAB, 0xEA, 0xAA},
{0xAB, 0xEA, 0xAA}};

const unsigned char char_eight[14][3] = {{0xAF, 0xFF, 0xAA},
{0xBF, 0xFF, 0xEA}, {0xFE, 0xAB, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFE, 0xAB, 0xFA}, {0xBE, 0xAB, 0xEA}, {0xAF, 0xFF, 0xAA},
{0xAF, 0xFF, 0xAA}, {0xBE, 0xAB, 0xEA}, {0xFE, 0xAB, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFE, 0xAB, 0xFA}, {0xBF, 0xFF, 0xEA},
{0xAF, 0xFF, 0xAA}};

const unsigned char char_nine[14][3] = {{0xAB, 0xFE, 0xAA},
{0xBF, 0xFF, 0xEA}, {0xFE, 0xAB, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFE, 0xAB, 0xFA},
{0xBF, 0xFF, 0xFA}, {0xAB, 0xFE, 0xFA}, {0xAA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFE, 0xAB, 0xFA}, {0xBF, 0xFF, 0xEA},
{0xAB, 0xFE, 0xAA}};

/* These three are exceptions */
const unsigned char char_period[14] = {0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xFA, 0xFA};

const unsigned char char_colon[14] = {0xAA, 0xAA, 0xAA,
0xFA, 0xFA, 0xAA, 0xAA, 0xAA, 0xAA, 0xFA, 0xFA, 0xAA, 0xAA, 0xAA};

const unsigned char char_short_space[14] = {0xAA, 0xAA, 0xAA,
0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

const unsigned char char_space[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}};

const unsigned char char_rarrow[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAB, 0xEA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xBE, 0xAA},
{0xAA, 0xAF, 0xAA}, {0xAA, 0xAB, 0xEA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}, {0xAA, 0xAB, 0xEA}, {0xAA, 0xAF, 0xAA},
{0xAA, 0xBE, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAB, 0xEA, 0xAA},
{0xAA, 0xAA, 0xAA}};

const unsigned char char_cube[14][3] = {{0xAF, 0xFA, 0xAA},
{0xFF, 0xFF, 0xAA}, {0xFA, 0xAF, 0xAA}, {0xAA, 0xAF, 0xAA},
{0xAA, 0xAF, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xAF, 0xAA}, {0xAA, 0xAF, 0xAA}, {0xFA, 0xAF, 0xAA},
{0xFF, 0xFF, 0xAA}, {0xAF, 0xFA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}};

const unsigned char char_a[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAB, 0xFE, 0xFA},
{0xBF, 0xFF, 0xFA}, {0xFE, 0xAB, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFE, 0xAB, 0xFA}, {0xBF, 0xFF, 0xFA},
{0xAB, 0xFE, 0xFA}};

const unsigned char char_C[14][3] = {{0xAB, 0xFE, 0xAA},
{0xAF, 0xFF, 0xAA}, {0xBF, 0xAF, 0xEA}, {0xBE, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xAA}, {0xFA, 0xAA, 0xAA},
{0xFA, 0xAA, 0xAA}, {0xFA, 0xAA, 0xAA}, {0xFA, 0xAA, 0xFA},
{0xBE, 0xAA, 0xFA}, {0xBF, 0xAF, 0xEA}, {0xAF, 0xFF, 0xAA},
{0xAB, 0xFE, 0xAA}};

const unsigned char char_e[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAB, 0xFE, 0xAA}, {0xBF, 0xFF, 0xEA},
{0xFE, 0xAB, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFA, 0xAA, 0xAA}, {0xFE, 0xAB, 0xFA}, {0xBF, 0xFF, 0xEA},
{0xAB, 0xFE, 0xAA}};

const unsigned char char_f[14][3] = {{0xAA, 0xBF, 0xAA},
{0xAA, 0xFB, 0xEA}, {0xAB, 0xFB, 0xFA}, {0xAB, 0xEA, 0xFA},
{0xAB, 0xEA, 0xAA}, {0xAB, 0xEA, 0xAA}, {0xBF, 0xFE, 0xAA},
{0xBF, 0xFE, 0xAA}, {0xAB, 0xEA, 0xAA}, {0xAB, 0xEA, 0xAA},
{0xAB, 0xEA, 0xAA}, {0xAB, 0xEA, 0xAA}, {0xAB, 0xEA, 0xAA},
{0xAB, 0xEA, 0xAA}};

const unsigned char char_g[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAF, 0xFF, 0xFA},
{0xBF, 0xFF, 0xFA}, {0xFE, 0xAB, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFE, 0xAB, 0xFA}, {0xBF, 0xFF, 0xFA}, {0xAF, 0xFE, 0xFA},
{0xAA, 0xAA, 0xFA}, {0xFE, 0xAB, 0xFA}, {0xBF, 0xFF, 0xEA},
{0xAB, 0xFE, 0xAA}};

const unsigned char char_i[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAB, 0xFE, 0xAA},
{0xAB, 0xFE, 0xAA}};

const unsigned char char_I[14][3] = {{0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}};

const unsigned char char_l[14][3] = {{0xAF, 0xFA, 0xAA},
{0xAF, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA},
{0xAA, 0xFA, 0xAA}, {0xAA, 0xFA, 0xAA}, {0xAF, 0xFF, 0xAA},
{0xAF, 0xFF, 0xAA}};

const unsigned char char_m[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xBF, 0xAF, 0xEA},
{0xFF, 0xAF, 0xFA}, {0xFA, 0xFA, 0xFA}, {0xFA, 0xFA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}};

const unsigned char char_o[17][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAB, 0xFE, 0xAA},
{0xBF, 0xFF, 0xEA}, {0xFE, 0xAB, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFE, 0xAB, 0xFA}, {0xBF, 0xFF, 0xEA},
{0xAB, 0xFE, 0xAA}};

const unsigned char char_P[14][3] = {{0xFF, 0xFF, 0xAA},
{0xFF, 0xFF, 0xEA}, {0xFA, 0xAB, 0xEA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAB, 0xEA}, {0xFF, 0xFF, 0xAA},
{0xFF, 0xFF, 0xAA}, {0xFA, 0xAA, 0xAA}, {0xFA, 0xAA, 0xAA},
{0xFA, 0xAA, 0xAA}, {0xFA, 0xAA, 0xAA}, {0xFA, 0xAA, 0xAA},
{0xFA, 0xAA, 0xAA}};

const unsigned char char_r[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xFB, 0xFE, 0xAA},
{0xFF, 0xFF, 0xEA}, {0xFE, 0xAB, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xAA}, {0xFA, 0xAA, 0xAA}, {0xFA, 0xAA, 0xAA},
{0xFA, 0xAA, 0xAA}};


const unsigned char char_R[14][3] = {{0xFF, 0xFF, 0xAA},
{0xFF, 0xFF, 0xEA}, {0xFA, 0xAB, 0xEA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAB, 0xEA}, {0xFF, 0xFF, 0xAA},
{0xFF, 0xFF, 0xAA}, {0xFA, 0xAB, 0xEA}, {0xFA, 0xAB, 0xEA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}};

const unsigned char char_t[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAF, 0xAA, 0xAA},
{0xAF, 0xAA, 0xAA}, {0xFF, 0xFA, 0xAA}, {0xFF, 0xFA, 0xAA},
{0xAF, 0xAA, 0xAA}, {0xAF, 0xAA, 0xAA}, {0xAF, 0xAA, 0xAA},
{0xAF, 0xAA, 0xFA}, {0xAB, 0xEB, 0xEA}, {0xAA, 0xFF, 0xAA},
{0xAA, 0xBE, 0xAA}};

const unsigned char char_u[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xBE, 0xAB, 0xEA}, {0xBF, 0xFF, 0xEA},
{0xAB, 0xFE, 0xAA}};

const unsigned char char_n[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xFB, 0xFE, 0xAA},
{0xFF, 0xFF, 0xEA}, {0xFE, 0xAB, 0xEA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}};

const unsigned char icon_bat_full[14][3] = {{0xAB, 0xFE, 0xAA},
{0xAB, 0xFE, 0xAA}, {0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}};

const unsigned char icon_bat_half[14][3] = {{0xAB, 0xFE, 0xAA},
{0xAB, 0xFE, 0xAA}, {0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}};

const unsigned char icon_bat_low[14][3] = {{0xAB, 0xFE, 0xAA},
{0xAB, 0xFE, 0xAA}, {0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}}; 

const unsigned char icon_gsm_no_signal[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}};

const unsigned char icon_gsm_signal_1[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xFE, 0xAA, 0xAA},
{0xFE, 0xAA, 0xAA}, {0xFE, 0xAA, 0xAA}, {0xFE, 0xAA, 0xAA},
{0xFE, 0xAA, 0xAA}, {0xFE, 0xAA, 0xAA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}};

const unsigned char icon_gsm_signal_2[14][3] = {{0xAA, 0xAA, 0xAA},
{0xAA, 0xAA, 0xAA}, {0xAA, 0xAA, 0xAA}, {0xAB, 0xFA, 0xAA},
{0xAB, 0xFA, 0xAA}, {0xAB, 0xFA, 0xAA}, {0xFF, 0xFA, 0xAA},
{0xFF, 0xFA, 0xAA}, {0xFF, 0xFA, 0xAA}, {0xFF, 0xFA, 0xAA},
{0xFF, 0xFA, 0xAA}, {0xFF, 0xFA, 0xAA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}};

const unsigned char icon_gsm_signal_3[14][3] = {{0xAA, 0xAF, 0xFA},
{0xAA, 0xAF, 0xFA}, {0xAA, 0xAF, 0xFA}, {0xAB, 0xFF, 0xFA},
{0xAB, 0xFF, 0xFA}, {0xAB, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}};

const unsigned char icon_log_off[14][3] = {{0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}};

const unsigned char icon_log_on[14][3] = {{0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFB, 0xFE, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFB, 0xFE, 0xFA},
{0xFA, 0xAA, 0xFA}, {0xFB, 0xFE, 0xFA}, {0xFA, 0xAA, 0xFA},
{0xFB, 0xFE, 0xFA}, {0xFA, 0xAA, 0xFA}, {0xFF, 0xFF, 0xFA},
{0xFF, 0xFF, 0xFA}};

const unsigned char *icons[9] = {icon_bat_full, icon_bat_half, icon_bat_low,
icon_gsm_no_signal, icon_gsm_signal_1, icon_gsm_signal_2, icon_gsm_signal_3,
icon_log_off, icon_log_on};

const unsigned char *numerals[10] = {char_zero, char_one, char_two,
char_three, char_four, char_five, char_six, char_seven, char_eight, char_nine};

const unsigned char *top_menu_run_str[4] = {char_space, char_R, char_u, char_n};

/* Strings for Program menu option */
const unsigned char *top_menu_prog_str[8] = {char_space, char_P, char_r, char_o,
char_g, char_r, char_a, char_m};
const unsigned char *prog_config_str[14] = {char_space, char_C, char_o, char_n,
char_f, char_i, char_g, char_u, char_r, char_a, char_t, char_i, char_o, char_n};

const unsigned char *top_menu_clr_str[6] = {char_space, char_C, char_l, char_e, char_a, char_r};

const unsigned char *top_menu_info_str[5] = {char_space, char_I, char_n, char_f, char_o};


unsigned char icon_row_list[16];
unsigned char date_time_list[10];




/* FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* UpdateDisplay ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* This function updates the E-ink display.
 * Pervasive Displays document no. 4P018-00 rev. 3. */

void EPD_Update(char start_line, char end_line){

    char stage, line, frame, row_line; //, x, y;
   
    /* Write display data to the EPD */
    /* ############################# */
    
  

        // new code for testing characters
        

        for(frame = 1; frame <= 4; frame++){ // repeat frames?
            
            InitScanData(start_line);
            row_line = 0;

            for(line = start_line; line <= end_line; line++){

                while(DISP_BUSY);
                SetDisplayLine(row_line);

                EPD_SPI_Send(0x0A, &display_line_new, 75);
                EPD_SPI_SendByte(0x02, 0x07);   // OE on
                ShiftScanData();
                
                row_line++;
                
                if(row_line > 13){
                    row_line = 0;
                }
            }
        }
        
        //Delay1ms(1400); // takes this long to do a stage??
   // }
}

/* It is required to call InitScanData() before using this function the
 * first time to ensure scan bytes 27 to 50 are initialized correctly. */
void SetDisplayLine(unsigned char line){
    
    unsigned char i, j;
    
    // set up the odd dot bytes in reverse order  
    display_line_new[0] = 0; // border byte
    i = 1;
    j = 49;
    
    while(i <= 25){
        display_line_new[i] = ((display_row[line][j] << 4) & 0xC0);
        display_line_new[i] |= ((display_row[line][j] >> 2) & 0x30);
        j--;
        display_line_new[i] |= display_row[line][j] & 0x0C;
        display_line_new[i] |= display_row[line][j] >> 6;
        j--;
        i++;
    }
    
    // set up the even dot bytes 
    i = 50;
    j = 0;
    
    while(i <= 74){
        display_line_new[i] = ((display_row[line][j] << 2) & 0xC0);
        display_line_new[i] |= ((display_row[line][j] << 4) & 0x30);
        j++;
        display_line_new[i] |= ((display_row[line][j] >> 2) & 0x0C);
        display_line_new[i] |= display_row[line][j] & 0x03;
        j++;
        i++;
    }
    
}

void EPD_PowerOff(void){

    char y;

    /* Write a 'nothing' frame */
    InitScanData(1);
    InitDisplayLines(0x00, 0x00);

    for(y = 1; y <= 96; y++){

        while(DISP_BUSY);

        EPD_SPI_Send(0x0A, &display_line_new, 75);
        EPD_SPI_SendByte(0x02, 0x07);   // OE on
        ShiftScanData();
        
    } 

    /* Write a border dummy line */
    InitScanData(1);
    display_line_new[49] = 0;
    InitDisplayLines(0x00, 0x00);
    display_line_new[0] = 0xAA;
    EPD_SPI_Send(0x0A, &display_line_new, 75);
    //EPD_SPI_Send(0x0A, &scan_data, 24);
    //EPD_SPI_Send(0x0A, &display_line_new_even, 25);
    EPD_SPI_SendByte(0x02, 0x07);   // OE on
    Delay1ms(200);

    EPD_SPI_SendByte(0x0B, 0x00);

    /* Latch reset ON */
    EPD_SPI_SendByte(0x03, 0x01);

    /* Power off chargepump Vcom */
    EPD_SPI_SendByte(0x05, 0x03);

    /* Power off neg. chargepump */
    EPD_SPI_SendByte(0x05, 0x01);
    Delay1ms(300);
    //Delay1ms(120);

    /* Discharge internal */
    EPD_SPI_SendByte(0x04, 0x80);

    /* Turn off all chargepumps */
    EPD_SPI_SendByte(0x05, 0x00);

    /* Turn off osc */
    EPD_SPI_SendByte(0x07, 0x01);
    Delay1ms(50);

    SPI2STATbits.SPIEN = 0; // disable SPI2 module
    ENABLE = 0;         // switch display power OFF
    Delay1ms(10);
    nDISP_CS = 0;
    nDISP_RESET = 0;
    DISCHARGE = 1;
    Delay1ms(150);
    DISCHARGE = 0;
}

/* EPD_SPI_Write ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void EPD_SPI_Write(unsigned char data){
    EPD_SPI_Read(data);
}


/* EPD_SPI_Read +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

unsigned char EPD_SPI_Read(unsigned char data){

    unsigned char temp;

    SPI2BUF = data;

    while(!IFS2bits.SPI2IF);
    while(!SPI2STATbits.SPIRBF);

    IFS2bits.SPI2IF = 0;
    temp = SPI2BUF;
    return temp;
}


void EPD_SPI_SendByte(unsigned char cog_register, unsigned char data){
    nDISP_CS = 0;
    EPD_SPI_Write(0x70); // header for register Index
    EPD_SPI_Write(cog_register);
    nDISP_CS = 1;
    Delay10us();
    nDISP_CS = 0;
    EPD_SPI_Write(0x72); // header for data
    EPD_SPI_Write(data);
    nDISP_CS = 1;
}



void EPD_SPI_Send(unsigned char cog_register, unsigned char *p_disp_data,
               unsigned char length){

    char index;

    nDISP_CS = 0;
    EPD_SPI_Write(0x70); // header for register index
    EPD_SPI_Write(cog_register);
    nDISP_CS = 1;
    Delay10us ();
    nDISP_CS = 0;
    EPD_SPI_Write(0x72); // header for data

    while(length--) {
        while(DISP_BUSY);
        EPD_SPI_Write(*p_disp_data++);
    }

    nDISP_CS = 1;
}


/* SPI_RID ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* The function name is the same as used in the EPD documentation. */
unsigned char SPI_RID(void){
    unsigned char result;

    nDISP_CS = 0;
    EPD_SPI_Write(0x71); // header for register Index
    result = EPD_SPI_Read(0x00);
    nDISP_CS = 1;
    return result;
}

/* SPI_R ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* The function name is the same as used in the EPD documentation. */

unsigned char SPI_R(unsigned char cog_register, unsigned char data){
    
    unsigned char temp, result;

    temp = data;
    nDISP_CS = 0;
    EPD_SPI_Write(0x70);    // header for register index
    EPD_SPI_Write(cog_register);
    nDISP_CS = 1;
    Delay10us();
    nDISP_CS = 0;
    EPD_SPI_Write(0x73);    // header for read command
    result = EPD_SPI_Read(temp);
    nDISP_CS = 1;
    return result;
}

/* For testing */
void InitScanData(char position){

    char i;

   // for(i = 0; i <= 22; i++){
     //   scan_data[i] = 0;
    //}

       
    for(i = 26; i <= 49; i++){
        display_line_new[i] = 0;
    }
    
    display_line_new[49] = 0x03;   // set the last two bits in the array
    scan = 23;
    
    if (position != 1){
        // shift the scan bytes if display lines start at position other than 1
        for (i = 1; i < position; i++){
           ShiftScanData(); 
        }
    }
}


void InitDisplayLines(char odd, char even){

    char i, j;
    
    // set up the odd bytes   
    display_line_new[0] = 0; // border byte
    
    for(i = 1; i <= 25; i++){
        display_line_new[i] = odd;
    }
    
        
    // set up the even bytes    
    for(i = 50; i <= 74; i++){
        display_line_new[i] = even;
    }
    
    
}


void CopyNewImage(void){
    
    char i;
    
    i = 0;
    
    while(i < 75){
        display_line_old[i] = display_line_new[i];
        i++;
    }
}


void ShiftScanData(void){
    
    if(display_line_new[scan+26] == 0xC0){
        // do next byte
        display_line_new[scan+26] = 0;
        
        if(scan != 0){
            display_line_new[scan+25] = 0x03;
            scan--;
        }    
    }else{
        display_line_new[scan+26]<<=2;
    }
}


void EPD_Init(void){
    
    char i;
    
    SPI2STATbits.SPIEN = 1; // enable SPI2 module
    SPI2STATbits.SPIROV = 0;
    IFS2bits.SPI2IF = 0;

    /* G2 COG driver power ON */
    /* ###################### */
    ENABLE = 1;         // switch display power ON
    Delay1ms(5);
    
    nDISP_CS = 1;
    nDISP_RESET = 1;
    Delay1ms(5);
    
    nDISP_RESET = 0;
    Delay1ms(5);
    
    nDISP_RESET = 1;
    Delay1ms(5);

    /* G2 COG driver initialization */
    /* ############################ */
    while(DISP_BUSY);

    /* Check COG ID */
    cog_data = SPI2BUF;         // clear the SPIRBF bit
    cog_data = SPI_RID();       // get ID
    //cog_data = SPI_R(0x72, 0x00);

    /* Do something here if ID != 2? */

    /* Disable OE */
    EPD_SPI_SendByte(0x02, 0x04);
    //Delay1ms(5);

    /* Check breakage */
    cog_data = SPI_R(0x0F, 0x00);

    /* Do something here if result !=0x80? */

    /* Power saving mode */
    EPD_SPI_SendByte(0x0B, 0x02);

    /* Channel select */
    EPD_SPI_Send(0x01, &epd_data, 8);
    //for(i = 0; i <= 7; i++){
        //EPD_SPI_SendByte(0x01, epd_data[i]);
    //}

    /* High Power Mode Osc Setting */
    EPD_SPI_SendByte(0x07, 0xD1);

    /* Power Setting */
    EPD_SPI_SendByte(0x08, 0x02);

    /* Set Vcom level */
    EPD_SPI_SendByte(0x09, 0xC2);

    /* Power Setting */
    EPD_SPI_SendByte(0x04, 0x03);

    /* Driver latch on */
    EPD_SPI_SendByte(0x03, 0x01);

    /* Driver latch off */
    EPD_SPI_SendByte(0x03, 0x00);
    Delay1ms(5);

    /* Chargepump start */
    i = 1;

    while(i < 4){
        /* Start chargepump V+ */
        EPD_SPI_SendByte(0x05, 0x01);
        Delay1ms(150);
        //Delay1ms(240);

        /* Start chargepump V- */
        EPD_SPI_SendByte(0x05, 0x03);
        Delay1ms(90);
        //Delay1ms(40);

        /* Set chargepump Vcom driver ON */
        EPD_SPI_SendByte(0x05, 0x0F);
        Delay1ms(40);

        /* CheckDC/DC */
        cog_data = SPI_R(0x0F, 0x00);

        if((cog_data & 0x40) == 0x40){
            EPD_SPI_SendByte(0x02, 0x06);
            //EPD_SPI_SendByte(0x02, 0x04);   // output enable to disable
            break;
        }

        i++;
    }
    
    if(i == 4){
        while(1);   // set breakpoint here for debugging
    }
}

/* Set up the characters to display in a row. The display_mode argument
 * depends on the menu state. */
void SetUpDisplayRow(char row_number, char menu_level){ //, unsigned char *digit_ptr){
    
    char i, j, k, y, x; //, *icon_ptr;
    
    switch(menu_level){
        case TOP_LEVEL:    
            switch(row_number){
                case 2:
                    // 2nd row                        
                    j = 0;

                    for(y = 0; y <= 13; y++){
                        k = 0;

                        for(x = 0; x < 3; x++){
                            if(menu_row_pointer == RUN_OPT){
                                display_row[y][x] = *(char_rarrow[0] + j + k);
                            }
                            else{
                                display_row[y][x] = *(top_menu_run_str[0] + j + k);
                            }

                            k++;
                        }

                        k = 0;

                        for(x = 3; x < 6; x++){
                            display_row[y][x] = *(top_menu_run_str[1] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 6; x < 9; x++){
                            display_row[y][x] = *(top_menu_run_str[2] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 9; x < 12; x++){
                            display_row[y][x] = *(top_menu_run_str[3] + j + k);
                            k++;
                        }

                        j += 3;

                        for(x = 12; x <= 49; x++){
                            display_row[y][x] = WHITE;
                        }
                    }
                    break;

                case 3:
                    // 3rd row                        
                    j = 0;

                    for(y = 0; y <= 13; y++){
                        k = 0;

                        for(x = 0; x < 3; x++){
                            if(menu_row_pointer == PROG_OPT){
                                display_row[y][x] = *(char_rarrow[0] + j + k);
                            }
                            else{
                                display_row[y][x] = *(top_menu_prog_str[0] + j + k);
                            }    
                            k++;
                        }

                        k = 0;

                        for(x = 3; x < 6; x++){
                            display_row[y][x] = *(top_menu_prog_str[1] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 6; x < 9; x++){
                            display_row[y][x] = *(top_menu_prog_str[2] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 9; x < 12; x++){
                            display_row[y][x] = *(top_menu_prog_str[3] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 12; x < 15; x++){
                            display_row[y][x] = *(top_menu_prog_str[4] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 15; x < 18; x++){
                            display_row[y][x] = *(top_menu_prog_str[5] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 18; x < 21; x++){
                            display_row[y][x] = *(top_menu_prog_str[6] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 21; x < 24; x++){
                            display_row[y][x] = *(top_menu_prog_str[7] + j + k);
                            k++;
                        }

                        j += 3;

                        for(x = 24; x <= 49; x++){
                            display_row[y][x] = WHITE;
                        }
                    }
                    break;    

                case 4:
                    // 4th row                        
                    j = 0;

                    for(y = 0; y <= 13; y++){
                        k = 0;

                        for(x = 0; x < 3; x++){
                            if(menu_row_pointer == CLR_OPT){
                                display_row[y][x] = *(char_rarrow[0] + j + k);
                            }
                            else{
                                display_row[y][x] = *(top_menu_clr_str[0] + j + k);
                            } 
                            
                            k++;
                        }

                        k = 0;

                        for(x = 3; x < 6; x++){
                            display_row[y][x] = *(top_menu_clr_str[1] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 6; x < 9; x++){
                            display_row[y][x] = *(top_menu_clr_str[2] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 9; x < 12; x++){
                            display_row[y][x] = *(top_menu_clr_str[3] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 12; x < 15; x++){
                            display_row[y][x] = *(top_menu_clr_str[4] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 15; x < 18; x++){
                            display_row[y][x] = *(top_menu_clr_str[5] + j + k);
                            k++;
                        }

                        j += 3;

                        for(x = 18; x <= 49; x++){
                            display_row[y][x] = WHITE;
                        }
                    }
                    break;  

                case 5:
                    // 5th row                        
                    j = 0;

                    for(y = 0; y <= 13; y++){
                        k = 0;

                        for(x = 0; x < 3; x++){
                            if(menu_row_pointer == INFO_OPT){
                                display_row[y][x] = *(char_rarrow[0] + j + k);
                            }
                            else{
                                display_row[y][x] = *(top_menu_info_str[0] + j + k);
                            } 
                            
                            k++;
                        }

                        k = 0;

                        for(x = 3; x < 6; x++){
                            display_row[y][x] = *(top_menu_info_str[1] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 6; x < 9; x++){
                            display_row[y][x] = *(top_menu_info_str[2] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 9; x < 12; x++){
                            display_row[y][x] = *(top_menu_info_str[3] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 12; x < 15; x++){
                            display_row[y][x] = *(top_menu_info_str[4] + j + k);
                            k++;
                        }

                        j += 3;

                        for(x = 15; x <= 49; x++){
                            display_row[y][x] = WHITE;
                        }
                    }
                    break;    
            }
            
            break;  // end of TOP_LEVEL case
    
        case SUB_LEVEL1:
            /* These strings are all displayed on the 3rd row.
             * menu_row_pointer determines what is displayed */
            switch(menu_row_pointer){
                case RUN_OPT:
                    break;
                    
                case PROG_OPT:
                    j = 0;

                    for(y = 0; y <= 13; y++){
                        k = 0;

                        for(x = 0; x < 3; x++){
                            display_row[y][x] = *(prog_config_str[0] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 3; x < 6; x++){
                            display_row[y][x] = *(prog_config_str[1] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 6; x < 9; x++){
                            display_row[y][x] = *(prog_config_str[2] + j + k);
                            k++;
                        }

                        k = 0;

                        for(x = 9; x < 12; x++){
                            display_row[y][x] = *(prog_config_str[3] + j + k);
                            k++;
                        }
                        
                        k = 0;

                        for(x = 12; x < 15; x++){
                            display_row[y][x] = *(prog_config_str[4] + j + k);
                            k++;
                        }
                        
                        k = 0;

                        for(x = 15; x < 18; x++){
                            display_row[y][x] = *(prog_config_str[5] + j + k);
                            k++;
                        }
                        
                        k = 0;

                        for(x = 18; x < 21; x++){
                            display_row[y][x] = *(prog_config_str[6] + j + k);
                            k++;
                        }
                        
                        k = 0;

                        for(x = 21; x < 24; x++){
                            display_row[y][x] = *(prog_config_str[7] + j + k);
                            k++;
                        }
                        
                        k = 0;

                        for(x = 24; x < 27; x++){
                            display_row[y][x] = *(prog_config_str[8] + j + k);
                            k++;
                        }
                        
                        k = 0;

                        for(x = 27; x < 30; x++){
                            display_row[y][x] = *(prog_config_str[9] + j + k);
                            k++;
                        }
                        
                        k = 0;

                        for(x = 30; x < 33; x++){
                            display_row[y][x] = *(prog_config_str[10] + j + k);
                            k++;
                        }
                        
                        k = 0;

                        for(x = 33; x < 36; x++){
                            display_row[y][x] = *(prog_config_str[11] + j + k);
                            k++;
                        }
                        
                        k = 0;

                        for(x = 36; x < 39; x++){
                            display_row[y][x] = *(prog_config_str[12] + j + k);
                            k++;
                        }
                        
                        k = 0;

                        for(x = 39; x < 42; x++){
                            display_row[y][x] = *(prog_config_str[13] + j + k);
                            k++;
                        }
                        
                        k = 0;

                        for(x = 42; x < 45; x++){
                            display_row[y][x] = *(prog_config_str[14] + j + k);
                            k++;
                        }

                        j += 3;

                        for(x = 45; x <= 49; x++){
                            display_row[y][x] = WHITE;
                        }
                    }
                    
                    break;
                    
                case CLR_OPT:
                    break;   
                    
                case INFO_OPT:
                    break;    
                
            }
            
            break;
    }
    
}


/* The icon row is updated irrespective of the menu level */
void SetUpIconRow(void){
    
    char i, j, k, y, x;
    
    j = 0;

    for(y = 0; y <= 13; y++){
        k = 0;

        for(x = 0; x < 3; x++){ // set up the battery icon
            display_row[y][x] = *(icons[icon_row_list[0]] + j + k);
            k++;
        }

        display_row[y][x] = char_short_space[y];
        k = 0;

        for(x = 4; x < 7; x++){ // set up the GSM signal strength icon
            display_row[y][x] = *(icons[icon_row_list[1]] + j + k);
            k++;
        }

        display_row[y][x] = char_short_space[y];
        k = 0;

        for(x = 8; x < 11; x++){ // set up the log icon
            display_row[y][x] = *(icons[icon_row_list[2]] + j + k);
            k++;
        }

        // add four short spaces
        for(x = 11; x < 15; x++){
            display_row[y][x] = char_short_space[y];
        }

        // add the date/time characters
        k = 0;
        for(x = 15; x < 18; x++){ // set up the day 10
            display_row[y][x] = *(numerals[date_time_list[0]] + j + k);
            k++;
        }


        k = 0;
        for(x = 18; x < 21; x++){ // set up the day 1
            display_row[y][x] = *(numerals[date_time_list[1]] + j + k);
            k++;
        }

        display_row[y][x] = char_period[y];

        k = 0;
        for(x = 22; x < 25; x++){ // set up the month 10
            display_row[y][x] = *(numerals[date_time_list[2]] + j + k);
            k++;
        }

        k = 0;
        for(x = 25; x < 28; x++){ // set up the month 1
            display_row[y][x] = *(numerals[date_time_list[3]] + j + k);
            k++;
        }

        display_row[y][x] = char_period[y];

        k = 0;
        for(x = 29; x < 32; x++){ // set up the year 10
            display_row[y][x] = *(numerals[date_time_list[4]] + j + k);
            k++;
        }

        k = 0;
        for(x = 32; x < 35; x++){ // set up the year 1
            display_row[y][x] = *(numerals[date_time_list[5]] + j + k);
            k++;
        }

        display_row[y][x] = char_short_space[y];

        k = 0;
        for(x = 36; x < 39; x++){ // set up the hours 10
            display_row[y][x] = *(numerals[date_time_list[6]] + j + k);
            k++;
        }

        k = 0;
        for(x = 39; x < 42; x++){ // set up the hours 1
            display_row[y][x] = *(numerals[date_time_list[7]] + j + k);
            k++;
        }

        display_row[y][x] = char_colon[y];

        k = 0;
        for(x = 43; x < 46; x++){ // set up the minutes 10
            display_row[y][x] = *(numerals[date_time_list[8]] + j + k);
            k++;
        }

        k = 0;
        for(x = 46; x < 49; x++){ // set up the minutes 1
            display_row[y][x] = *(numerals[date_time_list[9]] + j + k);
            k++;
        }

        j += 3;
    }
    
}

/* Clear the row indicated by row. If row = 0, then clear entire display. */
void ClearDisplay(char row){
    
    char x, y, start, end;
    
    for(y = 0; y <= 13; y++){
        for(x = 0; x <= 49; x++){
            display_row[y][x] = WHITE;
        }
    }
    
    switch(row){
        case 0: // clear entire display
            start = 1;  //ROW1_START;
            end = 96;   //ROW5_END;
            break;
            
        case 1: // clear row 1 (icons)            
            start = ROW1_START;
            end = ROW1_END;
            break;
            
        case 2: // clear row 2            
            start = ROW2_START;
            end = ROW2_END;
            break;
            
        case 3: // clear row 3            
            start = ROW3_START;
            end = ROW3_END;
            break;
            
        case 4: // clear row 4            
            start = ROW4_START;
            end = ROW4_END;
            break;    
            
        case 5: // clear row 5            
            start = ROW5_START;
            end = ROW5_END;
            break;    
    }
    
    EPD_Init();
    EPD_Update(start, end);
    EPD_PowerOff();
}
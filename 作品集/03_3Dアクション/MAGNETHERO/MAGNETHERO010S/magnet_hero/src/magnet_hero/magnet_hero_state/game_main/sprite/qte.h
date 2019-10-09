#pragma once

//QTE‚Í–³‚­‚È‚è‚Ü‚µ‚½
#define qte_lost

#ifndef qte_lost
/*
*@fn qte_init
*/
extern void qte_init();

/*
*@fn qte_end
*/
extern void qte_end();

/*
*@fn qte_update
*/
extern void qte_update();

/*
*@fn qte_draw
*/
extern void qte_draw();

/*
*@fn qte_start
*/
extern void qte_start();

/*
*@fn qte_judge
*/
extern bool qte_judge();

/*
*@fn qte_timeout
*/
extern bool qte_timeout();
#else
#define qte_init()

#define qte_end()

#define qte_update()

#define qte_draw()

#define qte_start()

#define qte_judge()			false

#define qte_timeout()		false
#endif//qte_lost
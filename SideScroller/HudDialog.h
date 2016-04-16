#ifndef HUDDIALOG_H
#define HUDDIALOG_H
#include <SDL.h>
class CHudDialog
{
public:
	virtual void Render(SDL_Renderer *) = 0;
	CHudDialog();
	virtual ~CHudDialog();
};

#endif
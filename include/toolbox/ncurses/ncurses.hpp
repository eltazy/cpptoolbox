#ifndef _TOOLBOX_NCURSES_NCURSES_HPP_
#define _TOOLBOX_NCURSES_NCURSES_HPP_

#include <ncursesw/ncurses.h>

namespace toolbox
{
namespace ncurses
{

class ncurses
{
public:
	ncurses()
	{
		initscr();
		raw();
		noecho();
		start_color();
	}

	~ncurses()
	{
		endwin();
	}

	void refresh() const
	{
		::refresh();
	}
};

} // namespace ncurses
} // namespace toolbox

#endif // _TOOLBOX_NCURSES_NCURSES_HPP_
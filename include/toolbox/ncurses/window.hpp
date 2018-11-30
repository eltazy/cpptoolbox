#ifndef _TOOLBOX_NCURSES_WINDOW_HPP_
#define _TOOLBOX_NCURSES_WINDOW_HPP_

#include <ncursesw/ncurses.h>
#include <toolbox/cpp/named_type.hpp>
#include <toolbox/rect.hpp>
#include <tuple>

namespace toolbox
{
namespace ncurses
{
namespace layout
{
using max_height = toolbox::cpp::named_type<int, struct max_height_tag>;
using min_height = toolbox::cpp::named_type<int, struct min_height_tag>;
using max_width = toolbox::cpp::named_type<int, struct max_width_tag>;
} // namespace layout

class window
{
private:
	class draw_functions
	{
	public:
		draw_functions(WINDOW *w)
			: window_(w)
		{}

		int printw(const char *fmt, ...)
		{
			va_list vl;
			va_start(vl, fmt);
			int res = vwprintw(window_, fmt, vl);
			va_end(vl);

			return res;
		}

		int putc(int c)
		{
			::waddch(window_, c);
		}

		void clear()
		{
			wclear(window_);
		}

		void move(int x, int y)
		{
			wmove(window_, y, x);
		}

	private:
		WINDOW *window_;
	};

public:
	window()
		: window_ (newwin(1, 1, 0, 0))
		, rect_ {{0, 0}, {1, 1}}
	{}

	window(toolbox::rect<int> r)
		: window_ (newwin(r.height(), r.width(), r.top_left.y, r.top_left.x))
		, rect_ (r)
	{
		wrefresh(window_);
	}

	~window()
	{
		delwin(window_);
	}

	void background(int color)
	{
		wbkgd(window_, COLOR_PAIR(color));
	}

	draw_functions api() const
	{
		return draw_functions(window_);
	}

	void update()
	{
		// clear screen
		clear();

		// client repaint
		redraw();

		// show contents on screen
		refresh();
	}

	void clear()
	{
		::wclear(window_);
	}

	void refresh()
	{
		::wrefresh(window_);
	}

	virtual void redraw()
	{
	}

	virtual void keypressed(int)
	{
	}

	template<class T>
	void set_constraint(T c)
	{
		std::get<T>(constraints_) = c;
	}

	template<class T>
	T constraint() const
	{
		return std::get<T>(constraints_);
	}

	void move(int x, int y)
	{
		mvwin(window_, y, x);
		rect_.top_left.x = x;
		rect_.top_left.y = y;
	}

	void resize(int width, int height)
	{
		rect_.set_width(width);
		rect_.set_height(height);
		wresize(window_, height, width);
	}

    constexpr auto width() const
    {
        return rect_.width();
    }

    constexpr auto height() const
    {
        return rect_.height();
    }

private:
	WINDOW *window_;
	toolbox::rect<int> rect_;
	std::tuple<
		layout::max_height,
		layout::min_height,
		layout::max_width
	> constraints_;
};

} // namespace window
} // namespace ncurses

#endif // _TOOLBOX_NCURSES_WINDOW_HPP_

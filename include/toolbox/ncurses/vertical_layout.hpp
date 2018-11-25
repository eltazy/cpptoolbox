#ifndef _TOOLBOX_NCURSES_VERTICAL_LAYOUT_HPP_
#define _TOOLBOX_NCURSES_VERTICAL_LAYOUT_HPP_

#include <toolbox/ncurses/window.hpp>
#include <vector>
#include <memory>

namespace toolbox
{
namespace ncurses
{
namespace detail
{

struct element {
	int min, max;
	int calculated = 0;
};

inline void layout_elements(int spaceAvailable, std::vector<element> &elements)
{
	// TODO: only supports MAX size constraint at this moment

	// naivly place all elements
	auto spaceLeft = spaceAvailable;
	auto count = elements.size();
	for (auto &e : elements) {
		auto slotSpace = spaceLeft / count;

		if (e.max < slotSpace) {
			e.calculated = e.max;
		} else {
			e.calculated = slotSpace;
		}

		spaceLeft -= e.calculated;
		-- count;
	}

	if (spaceLeft <= 0)
		return; // may be enough

	for (auto &e : elements) {
		if (e.calculated == e.max)
			continue;
		
		e.calculated += spaceLeft;
	}
}

} // namespace detail

class vertical_layout
{
public:
	vertical_layout()
	{
	}

	vertical_layout(std::initializer_list<std::shared_ptr<toolbox::ncurses::window>> windows)
		: windows_{windows}
	{
	}

	void add(std::shared_ptr<toolbox::ncurses::window> window)
	{
		windows_.emplace_back(window);
	}

	void update()
	{
		std::vector<detail::element> layoutSpec(windows_.size());

		for (auto i = 0u; i < windows_.size(); ++i) {
			layoutSpec[i].min = 1;
			layoutSpec[i].max = windows_[i]->constraint<toolbox::ncurses::layout::max_height>() ? windows_[i]->constraint<toolbox::ncurses::layout::max_height>() : LINES;
		}

		detail::layout_elements(LINES, layoutSpec);

		auto line = 0;
		for (auto i = 0u; i < windows_.size(); ++i) {
			// WTF: will draw wrongly when call order (move, resize) is different
			windows_[i]->resize(COLS, layoutSpec[i].calculated);
			windows_[i]->move(0, line);
			windows_[i]->update();

			line += layoutSpec[i].calculated;
		}
	}

private:
	std::vector<std::shared_ptr<toolbox::ncurses::window>> windows_;
};
} // namespace ncurses
} // namespace toolbox

#endif // _TOOLBOX_NCURSES_VERTICAL_LAYOUT_HPP_
#pragma once

#include "../Includes.hpp"

#include <Geode/modify/CommentCell.hpp>

class $modify(ProCommentCell, CommentCell) {

	static void onModify(auto& self) {
        (void)self.setHookPriorityAfterPost("CommentCell::loadFromComment", "alphalaneous.fine_outline");
	}

	void loadFromComment(GJComment*);

};
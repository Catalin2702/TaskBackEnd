//
// Created by Catalin Chirosca on 23/12/24.
//
#include "api/model/m_category.hpp"

Category::Category():
	Model("categories") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerEmptyColumn<column::StringColumn>(title);
	registerEmptyColumn<column::TextColumn>(description);
	registerEmptyColumn<column::IntegerColumn>(userId);
	registerEmptyColumn<column::TimestampColumn>(created);
	registerEmptyColumn<column::TimestampColumn>(updated);
}

Category::Category(const std::string& title, const std::string& description, const int userId):
	Model("categories") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerColumn<column::StringColumn>(this->title, title);
	registerColumn<column::TextColumn>(this->description, description);
	registerColumn<column::IntegerColumn>(this->userId, userId);
	registerColumn<column::TimestampColumn>(created, std::chrono::system_clock::now());
	registerColumn<column::TimestampColumn>(updated, std::chrono::system_clock::now());
}
json Category::toJson() const {
	json categoryJson;
	categoryJson["id"] = id.getValue();
	categoryJson["title"] = title.getValue();
	categoryJson["description"] = description.getValue();
	categoryJson["userId"] = userId.getValue();
	categoryJson["created"] = tools::timestampToString(created.getValue());
	categoryJson["updated"] = tools::timestampToString(updated.getValue());
	return categoryJson;
}

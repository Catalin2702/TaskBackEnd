//
// Created by Catalin Chirosca on 23/12/24.
//
#include "api/model/m_categorie.hpp"

Categorie::Categorie():
	Model("categories") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerEmptyColumn<column::StringColumn>(title);
	registerEmptyColumn<column::TextColumn>(description);
	registerEmptyColumn<column::IntegerColumn>(userId);
	registerEmptyColumn<column::TimestampColumn>(created);
	registerEmptyColumn<column::TimestampColumn>(updated);
}

Categorie::Categorie(const std::string& title, const std::string& description, const int userId):
	Model("categories") {
	registerEmptyColumn<column::SerialColumn>(id);
	registerColumn<column::StringColumn>(this->title, title);
	registerColumn<column::TextColumn>(this->description, description);
	registerColumn<column::IntegerColumn>(this->userId, userId);
	registerColumn<column::TimestampColumn>(created, std::chrono::system_clock::now());
	registerColumn<column::TimestampColumn>(updated, std::chrono::system_clock::now());
}
//
// Created by Catalin Chirosca on 13/12/24.
//

#ifndef SESSION_HPP
#define SESSION_HPP

#include <memory>
#include <pqxx/pqxx>
#include <vector>
#include <type_traits>

#include "database.hpp"
#include "model.hpp"

namespace session {
	template<typename M>
	class Query;

	class Session {
	public:
		explicit Session(const database::Database& db);
		~Session();
		void commit();
		void rollback();
		template<typename M>
		Query<M> query(const M& model);
		template<typename M>
		Query<M> query(const std::vector<M>& models);
		template<typename M>
		std::vector<M> execute(const Query<M>& query);
		pqxx::result execute(const std::string& query);
	private:
		void beginTransaction();
		void ensureTransaction();
		database::Database db;
		std::unique_ptr<pqxx::work> transaction = nullptr;
	};

	class ConnectionMaker {
	public:
		explicit ConnectionMaker(const std::string& uri);
		[[nodiscard]] std::unique_ptr<Session> getSession() const;
	private:
		std::unique_ptr<database::Database> db;
	};

	template<typename M>
	class Query {
	public:
		static_assert(std::is_base_of_v<model::Model, M>, "Template must derive from Model!");

		Query(const Query& query);
		explicit Query(const M& model, Session* session);
		explicit Query(const std::vector<M>& models, Session* session);

		Query filter(const std::string& condition) const;
		template <typename T>
		Query filter(condition::Condition<T> condition) const;

		[[nodiscard]] std::optional<std::vector<M>> all();
		[[nodiscard]] std::optional<M> first();
		[[nodiscard]] std::optional<std::vector<M>> limit(const int limit);
		[[nodiscard]] std::optional<std::vector<M>> insert();
		[[nodiscard]] std::optional<M> update(const std::vector<std::string>& columns = {});
		[[nodiscard]] std::vector<unsigned long> remove();
		[[nodiscard]] std::string toString() const;
		explicit operator std::string() const;

	private:
		[[nodiscard]] std::vector<std::shared_ptr<column::ColumnBase>> getColumns() const;
		[[nodiscard]] std::vector<std::shared_ptr<column::ColumnBase>> getDirtyColumns() const;
		[[nodiscard]] std::vector<std::string> getColumnNames() const;
		[[nodiscard]] std::shared_ptr<column::ColumnBase> getPrimaryKey();
		[[nodiscard]] std::string buildSelectQuery(const unsigned int limit = 0);
		[[nodiscard]] std::string buildSingleInsertQuery() const;
		[[nodiscard]] std::string buildBatchInsertQuery() const;
		[[nodiscard]] std::string buildUpdateQuery(const std::vector<std::string>& columns = {});
		[[nodiscard]] std::string buildDeleteQuery();
		std::string condition;
		std::string finalQuery;
		bool isBatch;
		M model;
		std::vector<M> models;
		Session* session;
	};

}

#include "session.tpp"

#endif //SESSION_HPP

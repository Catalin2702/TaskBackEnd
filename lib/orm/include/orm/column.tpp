#pragma once

namespace value {

	template<typename T>
	EnumValue<T>::EnumValue(const std::map<T, std::string> &mapping, const T *value):
	SqlValue(SqlType::Enum), value(value ? *value : mapping.begin()->first), mapping(mapping) {
		if (mapping.find(*value) == mapping.end())
			throw std::invalid_argument("Value not in mapping");
	}
	template<typename T>
	std::string EnumValue<T>::toString() const {
		return mapping.at(value);
	}
	template<typename T>
	const std::map<T, std::string>& EnumValue<T>::getMapping() const {
		return mapping;
	}
	template<typename T>
	T* EnumValue<T>::getPtr() {
		return &value;
	}
	template<typename T>
	void EnumValue<T>::setValue(const T value) {
		if (not mapping.count(value))
			throw std::invalid_argument("Value " + toString() + " not in mapping.");
		this->value = value;
	}
	template <typename T>
	void EnumValue<T>::setMapping(const std::map<T, std::string>& mapping, const T* value) {
		this->mapping = mapping;
		if (value)
			setValue(*value);
		if (not mapping.count(value))
			throw std::invalid_argument("Value " + toString() + " not in mapping.");
	}
	template<typename T>
	constexpr bool EnumValue<T>::isStringLike() {
		static_assert(std::is_same_v<SqlValue, T>, "EnumValue must derive from SqlValue");
		return T::isStringLike();
	}


}

namespace condition {

	template <typename T>
	Condition<T>::Condition(const std::string& column, const SqlOperator op, const T value):
		column(column), op(op), values{value} {}
	template<typename T>
	Condition<T>::Condition(const std::string& column, const SqlOperator op, const std::vector<T>& values):
		column(column), op(op), values(values) {}
	template<typename T>
	Condition<T>::Condition(std::unique_ptr<ConditionBase> conditionBase):
		composition(std::move(conditionBase)), isComposite(true), op() {}
	template<typename T>
	Condition<T>::Condition(const std::string& column, const SqlOperator op, const T value, std::unique_ptr<ConditionBase> composition):
		column(column), op(op), values{value}, composition(std::move(composition)), isComposite(true) {}
	template<typename T>
	Condition<T>::Condition(const std::string& column, const SqlOperator op, const std::vector<T>& values, std::unique_ptr<ConditionBase> composition):
		column(column), op(op), values(values), composition(std::move(composition)), isComposite(true) {}
	template <typename T>
	std::string Condition<T>::toSql() const {
		if (isComposite and composition)
			return composition->toSql();
		const auto isString = std::is_same_v<T, std::string>;
		std::ostringstream result {};
		const std::string opStr = SqlOperatorDef[op];
		switch (op) {
			case SqlOperator::IN:
			case SqlOperator::NOT_IN:
				result << column << " " << opStr << " (" << tools::join(values, ", ", isString) << ")";
			break;
			case SqlOperator::IS_NULL:
			case SqlOperator::IS_NOT_NULL:
			case SqlOperator::IS_TRUE:
			case SqlOperator::IS_FALSE:
				result << column << " " << opStr;
			break;
			default:
				if constexpr (std::is_same_v<T, std::string>)
					result << column << " " << opStr << " '" << values[0] << "'";
				else
					result << column << " " << opStr << " " << std::to_string(values[0]);
		}
		return result.str();
	}
	template <typename T>
	std::string Condition<T>::toString() const {
		return toSql();
	}
	template <typename T>
	Condition<T>::operator std::string() const {
		return toString();
	}
	template <typename T>
	std::unique_ptr<ConditionBase> Condition<T>::clone() const {
		return std::make_unique<Condition>(column, op, values, composition ? composition->clone() : nullptr);
	}
	template <typename T>
	Condition<T> Condition<T>::operator && (const ConditionBase& other) const {
		auto composite = std::make_unique<CompositeCondition>(
			clone(),
			other.clone(),
			SqlOperator::AND
		);
		return Condition(std::move(composite));
	}
	template <typename T>
	Condition<T> Condition<T>::operator || (const ConditionBase& other) const {
		auto composite = std::make_unique<CompositeCondition>(
			clone(),
			other.clone(),
			SqlOperator::OR
		);
		return Condition(std::move(composite));
	}
	template <typename T>
	Condition<T> Condition<T>::operator!() const {
		auto composite = std::make_unique<CompositeCondition>(
			clone(),
			nullptr,
			SqlOperator::NOT
		);
		return Condition(std::move(composite));
	}

}

namespace column {

	template <typename T>
	T* ColumnBase::getPtrValue() const {
		return static_cast<T*>(getPtr());
	}
	template <typename T>
	T ColumnBase::getValue() const {
		return *getPtrValue<T>();
	}
	template<typename T>
	EnumColumn<T>::EnumColumn(const std::string &name, const std::map<T, std::string> mapping, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Enum, "", primaryKey, nullable) {
		this->value = std::make_unique<value::EnumValue<T>>(mapping);
	}
	template<typename T>
	EnumColumn<T>::EnumColumn(const std::string &name, const T value, const std::map<T, std::string> mapping, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Enum, "", primaryKey, nullable) {
		this->value = std::make_unique<value::EnumValue<T>>(mapping, value);
	}
	template <typename T>
	EnumColumn<T>::EnumColumn(const EnumColumn& other) :
		ColumnBase(other.getName(), other.getType(), other.getTableName(), other.isPrimaryKey(), other.isNullable()) {
		if (other.value)
			setValue(*other.value->getPtr());
	}
	template <typename T>
	EnumColumn<T>::~EnumColumn() {
		if (this->value)
			this->value.reset();
	}
	template <typename T>
	std::map<T, std::string> EnumColumn<T>::getMapping() const {
		return this->value ? this->value->getMapping() : std::map<T, std::string>();
	}
	template<typename T>
	T* EnumColumn<T>::getPtrValue() const {
		return static_cast<T*>(getPtr());
	}
	template<typename T>
	T EnumColumn<T>::getValue() const {
		if (auto ptr = getPtrValue())
			return *ptr;
		throw std::runtime_error("Enum value not set");
	}
	template<typename T>
	void EnumColumn<T>::setValue(const T value) {
		if (not this->value)
			throw std::runtime_error("Enum value not set");
		this->value->setValue(value);
		markDirty();
	}
	template<typename T>
	void EnumColumn<T>::setMapping(const std::map<T, std::string>& mapping, const T value) {
		if (not this->value)
			this->value = std::make_unique<value::EnumValue<T>>(mapping, value);
		else
			this->value->setMapping(mapping, value);
		markDirty();
	}
	template <typename T>
	std::string EnumColumn<T>::getValueAsString() const {
		if (auto ptr = getPtrValue()) {
			return getMapping().at(*ptr);
		}
		return "null";
	}
	template<typename T>
	void EnumColumn<T>::setNull(const bool init) {
		if (this->value)
			this->value.reset();
		if (not init)
			markDirty();
	}
	template<typename T>
	void EnumColumn<T>::setValueFromPtr(const void* ptr) {
		if (ptr == nullptr)
			setNull();
		else {
			T enumValue = *static_cast<const T*>(ptr);
			if (const auto mapping = getMapping(); mapping.find(enumValue) == mapping.end()) {
				throw std::invalid_argument("Value not in mapping");
			} else {
				value = std::make_unique<value::EnumValue<T>>(mapping, &enumValue);
			}
		}
		markDirty();
	}
	template<typename T>
	void EnumColumn<T>::initValue(const pqxx::field& field) {
		if (field.is_null())
			setNull(true);
		else {
			T enumValue = field.as<T>();
			if (const auto mapping = getMapping(); mapping.find(enumValue) == mapping.end()) {
				throw std::invalid_argument("Value not in mapping");
			} else {
				value = std::make_unique<value::EnumValue<T>>(mapping, &enumValue);
			}
		}
	}
	template<typename T>
	void* EnumColumn<T>::getPtr() const {
		return value ? value->getPtr() : nullptr;
	}
	template<typename T>
	std::unique_ptr<ColumnBase> EnumColumn<T>::clone() const {
		return std::make_unique<EnumColumn<T>>(*this);
	}
	template <typename T>
	constexpr bool EnumColumn<T>::isStringLike() {
		return T::isStringLike();
	}

	template<typename T>
	EnumColumn<T>& EnumColumn<T>::operator = (const T value) {
		setValue(value);
		return *this;
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::operator == (const T value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, this->value ? this->getMapping()[this->value] : ""};
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::operator == (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value};
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::operator != (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::operator != (const T value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::in(const std::vector<T>& values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::in(const std::vector<std::string>& values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::notIn(const std::vector<T>& values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::notIn(const std::vector<std::string>& values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::like(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::notLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::iLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	template<typename T>
	condition::Condition<std::string> EnumColumn<T>::notILike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}

	template<typename T>
	ColumnRef<T>::ColumnRef(std::shared_ptr<T> col): column(col) {
		static_assert(std::is_base_of_v<ColumnBase, T>, "T must be a subclass of ColumnBase");
	}
	template<typename T>
	ColumnRef<T>::operator std::shared_ptr<T>() {
		return column;
	}
	template<typename T>
	ColumnRef<T>::operator std::shared_ptr<T>&() {
		return column;
	}
	template<typename T>
	T* ColumnRef<T>::operator->() const {
		return column.get();
	}
	template<typename T>
	T& ColumnRef<T>::operator*() const {
		return *column;
	}
	template<typename T>
	ColumnRef<T>& ColumnRef<T>::operator = (const value_type& value) {
		column->setValue(value);
		return *this;
	}
	template<typename T>
	template<typename U>
	ColumnRef<T>& ColumnRef<T>::operator = (const char* value) {
		column->setValue(std::string(value));
		return *this;
	}
	template<typename T>
	condition::Condition<typename T::value_type> ColumnRef<T>::operator == (const value_type& value) const {
		return column->operator == (value);
	}
	template<typename T>
	condition::Condition<typename T::value_type> ColumnRef<T>::operator != (const value_type& value) const {
		return column->operator != (value);
	}
	template<typename T>
	condition::Condition<typename T::value_type> ColumnRef<T>::operator < (const value_type& value) const {
		return column->operator < (value);
	}
	template<typename T>
	condition::Condition<typename T::value_type> ColumnRef<T>::operator <= (const value_type& value) const {
		return column->operator <= (value);
	}
	template<typename T>
	condition::Condition<typename T::value_type> ColumnRef<T>::operator > (const value_type& value) const {
		return column->operator > (value);
	}
	template<typename T>
	condition::Condition<typename T::value_type> ColumnRef<T>::operator >= (const value_type& value) const {
		return column->operator >= (value);
	}
	template<typename T>
	std::shared_ptr<T>& ColumnRef<T>::get() {
		return column;
	}
	template<typename T>
	typename T::value_type* ColumnRef<T>::getPtrValue() const {
		return column->getPtrValue();
	}
	template<typename T>
	typename T::value_type ColumnRef<T>::getValue() const {
		return column->getValue();
	}
	template<typename T>
	std::string ColumnRef<T>::getValueAsString() const {
		return column->getValueAsString();
	}
	template<typename T>
	void ColumnRef<T>::setNull(const bool init) {
		column->setNull(init);
	}
	template<typename T>
	[[nodiscard]] static constexpr bool isStringLike() {
		return T::isStringLike();
	}
	template<typename T>
	template<typename U>
	condition::Condition<typename ColumnRef<T>::value_type>
	ColumnRef<T>::in(const std::vector<value_type>& values) const {
		return column->in(values);
	}
	template<typename T>
	template<typename U>
	condition::Condition<typename ColumnRef<T>::value_type>
	ColumnRef<T>::notIn(const std::vector<value_type>& values) const {
		return column->notIn(values);
	}
	template<typename T>
	template<typename U, typename std::enable_if<U::isStringLike(), int>::type>
	condition::Condition<std::string> ColumnRef<T>::like(const std::string& value) const {
		return column->like(value);
	}
	template<typename T>
	template<typename U, typename std::enable_if<U::isStringLike(), int>::type>
	condition::Condition<std::string> ColumnRef<T>::notLike(const std::string& value) const {
		return column->notLike(value);
	}
	template<typename T>
	template<typename U, typename std::enable_if<U::isStringLike(), int>::type>
	condition::Condition<std::string> ColumnRef<T>::iLike(const std::string& value) const {
		return column->iLike(value);
	}
	template<typename T>
	template<typename U, typename std::enable_if<U::isStringLike(), int>::type>
	condition::Condition<std::string> ColumnRef<T>::notILike(const std::string& value) const {
		return column->notILike(value);
	}

}

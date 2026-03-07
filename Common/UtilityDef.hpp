#define ENUM_CLASS_BITWISE_OPS_DEC(Class, Underlying, Qualif) \
	Qualif Underlying EnumUnderlying(Class e) noexcept; \
	Qualif bool EnumIsValueSet(Class e, Underlying value) noexcept; \
	Qualif Class operator|(Class lhs, Class rhs) noexcept; \
	Qualif Class operator|(Class lhs, Underlying rhs) noexcept; \
	Qualif Class operator&(Class lhs, Class rhs) noexcept; \
	Qualif Class operator&(Class lhs, Underlying rhs) noexcept; \
	Qualif Class& operator|=(Class& lhs, Class rhs) noexcept; \
	Qualif Class operator~(Class e) noexcept; \
	Qualif bool operator==(Class e, Underlying value) noexcept;

#define ENUM_CLASS_BITWISE_OPS_DEF(Class, Underlying, Qualif) \
	Qualif Underlying EnumUnderlying(Class e) noexcept \
	{ \
		return static_cast<Underlying>(e); \
	} \
	Qualif bool EnumIsValueSet(Class e, Underlying value) noexcept \
	{ \
		return e == value || EnumUnderlying(e & value); \
	} \
	\
	Qualif Class operator|(Class lhs, Class rhs) noexcept \
	{ \
		return static_cast<Class>( \
			EnumUnderlying(lhs) | \
			EnumUnderlying(rhs) \
		); \
	} \
	Qualif Class operator|(Class lhs, Underlying rhs) noexcept \
	{ \
		return lhs | static_cast<Class>(rhs); \
	} \
    \
	\
	Qualif Class operator&(Class lhs, Class rhs) noexcept \
	{ \
		return static_cast<Class>( \
			EnumUnderlying(lhs) & \
			EnumUnderlying(rhs) \
		); \
	} \
	Qualif Class operator&(Class lhs, Underlying rhs) noexcept \
	{ \
		return lhs & static_cast<Class>(rhs); \
	} \
	\
	Qualif Class& operator|=(Class& lhs, Class rhs) noexcept \
	{ \
		lhs = lhs | rhs; \
		return lhs; \
	} \
	\
	Qualif Class operator~(Class e) noexcept \
	{ \
		return static_cast<Class>(~EnumUnderlying(e)); \
	} \
	Qualif bool operator==(Class e, Underlying value) noexcept \
	{ \
		return e == static_cast<Class>(value); \
	}

#define ENUM_CLASS_BITWISE_OPS_IMPL(Class, Underlying, Qualif) \
	ENUM_CLASS_BITWISE_OPS_DEC(Class, Underlying, Qualif) \
	ENUM_CLASS_BITWISE_OPS_DEF(Class, Underlying, Qualif)
// Filename: Module.ipp
namespace dclass   // open namespace
{


inline Import::Import(const std::string& module_name) :
	module(module_name)
{
}


// get_num_classes returns the number of classes in the module
inline size_t Module::get_num_classes() const
{
	return m_classes.size();
}
// get_class returns the <n>th class read from the dcfile(s).
inline Class* Module::get_class(unsigned int n)
{
	return m_classes.at(n);
}
inline const Class* Module::get_class(unsigned int n) const
{
	return m_classes.at(n);
}

// get_num_structs returns the number of structs in the module.
inline size_t Module::get_num_structs() const
{
	return m_structs.size();
}
// get_struct returns the <n>th struct in the module.
inline Struct* Module::get_struct(unsigned int n)
{
	return m_structs.at(n);
}
inline const Struct* Module::get_struct(unsigned int n) const
{
	return m_structs.at(n);
}

// get_num_types returns the number of types in the module.
//     All type ids will be within the range 0 <= id < get_num_types().
inline size_t Module::get_num_types() const
{
	return m_types_by_id.size();
}

// get_type_by_id returns the requested type or NULL if there is no such type.
inline DistributedType* Module::get_type_by_id(unsigned int id)
{
	if(id < m_types_by_id.size())
	{
		return m_types_by_id[id];
	}

	return (DistributedType*)NULL;
}
inline const DistributedType* Module::get_type_by_id(unsigned int id) const
{
	if(id < m_types_by_id.size())
	{
		return m_types_by_id[id];
	}

	return (const DistributedType*)NULL;
}
// get_type_by_name returns the requested type or NULL if there is no such type.
inline DistributedType* Module::get_type_by_name(const std::string &name)
{
	auto type_ref = m_types_by_name.find(name);
	if(type_ref != m_types_by_name.end())
	{
		return type_ref->second;
	}

	return (DistributedType*)NULL;
}
inline const DistributedType* Module::get_type_by_name(const std::string &name) const
{
	auto type_ref = m_types_by_name.find(name);
	if(type_ref != m_types_by_name.end())
	{
		return type_ref->second;
	}

	return (const DistributedType*)NULL;
}

// get_field_by_id returns the request field or NULL if there is no such field.
inline Field* Module::get_field_by_id(unsigned int id)
{
	if(id < m_fields_by_id.size())
	{
		return m_fields_by_id[id];
	}

	return (Field*)NULL;
}
inline const Field* Module::get_field_by_id(unsigned int id) const
{
	if(id < m_fields_by_id.size())
	{
		return m_fields_by_id[id];
	}

	return (const Field*)NULL;
}

// get_num_imports returns the number of imports in the module.
inline size_t Module::get_num_imports() const
{
	return m_imports.size();
}
// get_import retuns the <n>th import in the module.
inline Import* Module::get_import(unsigned int n)
{
	return m_imports.at(n);
}
inline const Import* Module::get_import(unsigned int n) const
{
	return m_imports.at(n);
}

// has_keyword returns true if a keyword with the name <keyword> is declared in the module.
inline bool Module::has_keyword(const std::string& keyword) const
{
	for(auto it = m_keywords.begin(); it != m_keywords.end(); ++it)
	{
		if(*it == keyword)
		{
			return true;
		}
	}
	return false;
}
// get_num_keywords returns the number of keywords declared in the module.
inline size_t Module::get_num_keywords() const
{
	return m_keywords.size();
}
// get_keyword returns the <n>th keyword declared in the module.
inline const std::string& Module::get_keyword(unsigned int n) const
{
	return m_keywords.at(n);
}


} // close namespace dclass

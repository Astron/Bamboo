// This file is just a dump of all the hash code extracted from the classes
// themselves and is not yet meant to be compiled.

uint32_t Module::hash() const
{
    HashGenerator hashgen;
    generate_hash(hashgen);
    return hashgen.hash();
}

// generate_hash accumulates the properties of this module into the hash.
void Module::generate_hash(HashGenerator& hashgen) const
{
    hashgen.add_int(m_classes.size());
    for(auto it = m_classes.begin(); it != m_classes.end(); ++it) {
        (*it)->generate_hash(hashgen);
    }

    hashgen.add_int(m_structs.size());
    for(auto it = m_structs.begin(); it != m_structs.end(); ++it) {
        (*it)->generate_hash(hashgen);
    }

    hashgen.add_int(m_keywords.size());
    for(auto it = m_keywords.begin(); it != m_keywords.end(); ++it) {
        hashgen.add_string(*it);
    }
}

// generate_hash accumulates the properties of this type into the hash.
void Array::generate_hash(HashGenerator& hashgen) const
{
    Type::generate_hash(hashgen);
    m_element_type->generate_hash(hashgen);
    if(has_range()) {
        hashgen.add_int(m_array_range.min.integer);
        hashgen.add_int(m_array_range.max.integer);
    } else {
        hashgen.add_int(m_array_size);
    }
}

// generate_hash accumulates the properties of this class into the hash.
void Class::generate_hash(HashGenerator& hashgen) const
{
    Type::generate_hash(hashgen);
    hashgen.add_string(m_name);

    /* Hash our inheritence tree */
    // We aren't the owener of our parents so we only add their id
    // We shouldn't hash our children because they aren't part of this class
    // and that relationship will be hashed when each child is hashed.
    hashgen.add_int(m_parents.size());
    for(auto it = m_parents.begin(); it != m_parents.end(); ++it) {
        hashgen.add_int((*it)->id());
    }

    /* Hash our constructor */
    if(m_constructor != nullptr) {
        m_constructor->generate_hash(hashgen);
    }

    /* Hash our declared fields */
    // We don't hash our inherited fields because thats implicit in the hash of our parents; also,
    // it is a bad idea to hash any thing we're not considered the owner of (could cause looping).
    hashgen.add_int(m_declared_fields.size());
    for(auto it = m_fields.begin(); it != m_fields.end(); ++it) {
        (*it)->generate_hash(hashgen);
    }
}

// generate_hash accumulates the properties of this field into the hash.
void Type::generate_hash(HashGenerator& hashgen) const
{
    hashgen.add_int(m_type);
    if(has_alias()) {
        hashgen.add_string(m_alias);
    }
}

// generate_hash accumulates the properties of this class into the hash.
void Struct::generate_hash(HashGenerator& hashgen) const
{
    Type::generate_hash(hashgen);
    hashgen.add_string(m_name);
    hashgen.add_int(m_fields.size());
    for(auto it = m_fields.begin(); it != m_fields.end(); ++it) {
        (*it)->generate_hash(hashgen);
    }
}

// generate_hash accumulates the properties of this type into the hash.
void Number::generate_hash(HashGenerator& hashgen) const
{
    Type::generate_hash(hashgen);
    hashgen.add_int(m_divisor);
    if(has_modulus()) {
        hashgen.add_int(m_modulus.integer);
    }
    if(has_range()) {
        hashgen.add_int(m_range.min.integer);
        hashgen.add_int(m_range.max.integer);
    }
}

// generate_hash accumulates the properties of this method into the hash
void Method::generate_hash(HashGenerator& hashgen) const
{
    Type::generate_hash(hashgen);
    hashgen.add_int(m_params.size());
    for(auto it = m_params.begin(); it != m_params.end(); ++it) {
        (*it)->generate_hash(hashgen);
    }
}


// generate_hash accumulates the properties of these keywords into the hash.
void KeywordList::generate_hash(HashGenerator& hashgen) const
{
    hashgen.add_int(m_keywords.size());
    for(auto it = m_keywords.begin(); it != m_keywords.end(); ++it) {
        hashgen.add_string(*it);
    }
}

// generate_hash accumulates the properties of this field into the hash.
void Field::generate_hash(HashGenerator& hashgen) const
{
    hashgen.add_int(m_id);
    hashgen.add_string(m_name);
    m_type->generate_hash(hashgen);
    KeywordList::generate_hash(hashgen);
}

// generate_hash accumulates the properties of this field into the hash.
void MolecularField::generate_hash(HashGenerator& hashgen) const
{
    hashgen.add_int(Field::m_id);
    hashgen.add_string(Field::m_name);
    // We aren't the owner of the fields so we only use their id in the hash
    hashgen.add_int(m_fields.size());
    for(auto it = m_fields.begin(); it != m_fields.end(); ++it) {
        hashgen.add_int((*it)->id());
    }
}

// generate_hash accumulates the properties of this type into the hash.
void Parameter::generate_hash(HashGenerator& hashgen) const
{
    m_type->generate_hash(hashgen);
}

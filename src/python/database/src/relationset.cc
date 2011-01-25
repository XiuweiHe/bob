/**
 * @author <a href="mailto:andre.dos.anjos@gmail.com">Andre Anjos</a> 
 * @date Fri 21 Jan 08:35:51 2011 
 *
 * @brief Bindings to Dataset::Relationset 
 */

#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "core/Dataset2.h"

using namespace boost::python;
namespace db = Torch::core;

static tuple get_relation_ids(const db::Relationset& rs) {
  list l;
  for (db::Relationset::const_iterator it=rs.begin(); it!=rs.end(); ++it) {
    l.append(it->first);
  }
  return tuple(l);
}

static tuple get_relations(const db::Relationset& rs) {
  list l;
  for (db::Relationset::const_iterator it=rs.begin(); it!=rs.end(); ++it) {
    l.append(it->second);
  }
  return tuple(l);
}

static tuple get_roles(const db::Relationset& rs) {
  list l;
  for (db::Relationset::rule_const_iterator it=rs.rule_begin(); it!=rs.rule_end(); ++it) {
    l.append(it->first);
  }
  return tuple(l);
}

static tuple get_rules(const db::Relationset& rs) {
  list l;
  for (db::Relationset::rule_const_iterator it=rs.rule_begin(); it!=rs.rule_end(); ++it) {
    l.append(it->second);
  }
  return tuple(l);
}

static const char* get_name(const db::Relationset& rs) {
  return rs.getName().c_str();
}

static void set_name(db::Relationset& rs, const char* name) {
  std::string n(name);
  rs.setName(n);
}

void bind_database_relationset() {
  class_<db::Relationset, boost::shared_ptr<db::Relationset> >("Relationset", "A Relationset describes groupings of Array/Arraysets in a Dataset.", init<>("Builds a new Relationset."))
    .def("append", &db::Relationset::addRelation, (arg("self"), arg("relation")))
    .def("append", &db::Relationset::addRule, (arg("self"), arg("rule")))
    .def("__getitem__", (boost::shared_ptr<db::Relation> (db::Relationset::*)(const size_t))&db::Relationset::getRelation, (arg("self"), arg("relation_id")))
    .def("__getitem__", (boost::shared_ptr<db::Rule> (db::Relationset::*)(const std::string&))&db::Relationset::getRule, (arg("self"), arg("rule_role")))
    .add_property("name", &get_name, &set_name, "This relationset's name")
    .add_property("relations", &get_relations, "All Relation's in this Relationset")    
    .add_property("roles", &get_roles, "All roles described in this Relationset")
    .add_property("rules", &get_rules, "All rules described in this Relationset")
    ;
}

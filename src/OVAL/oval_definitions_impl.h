/**
 * @file oval_definitions_impl.h
 * \brief Open Vulnerability and Assessment Language
 *
 * See more details at http://oval.mitre.org/
 */

/*
 * Copyright 2009 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors:
 *      "David Niemoller" <David.Niemoller@g2-inc.com>
 */

#ifndef OVAL_DEFINITIONS_IMPL
#define OVAL_DEFINITIONS_IMPL

#include <libxml/xmlreader.h>
#include "public/oval_definitions.h"
#include "oval_parser_impl.h"
#include "oval_agent_api_impl.h"
#include "oval_string_map_impl.h"
#include "../common/util.h"

OSCAP_HIDDEN_START;

oval_family_t oval_family_parse(xmlTextReaderPtr);
oval_subtype_t oval_subtype_parse(xmlTextReaderPtr);
oval_affected_family_t oval_affected_family_parse(xmlTextReaderPtr);
oval_operator_t oval_operator_parse(xmlTextReaderPtr, char *, oval_operator_t);
oval_operation_t oval_operation_parse(xmlTextReaderPtr, char *, oval_operation_t);
oval_definition_class_t oval_definition_class_parse(xmlTextReaderPtr);
oval_existence_t oval_existence_parse(xmlTextReaderPtr, char *, oval_existence_t);
oval_check_t oval_check_parse(xmlTextReaderPtr, char *, oval_check_t);
oval_entity_type_t oval_entity_type_parse(xmlTextReaderPtr);
oval_datatype_t oval_datatype_parse(xmlTextReaderPtr, char *, oval_datatype_t);
oval_entity_varref_type_t oval_entity_varref_type_parse(xmlTextReaderPtr);
oval_setobject_type_t oval_set_type_parse(xmlTextReaderPtr);
oval_setobject_operation_t oval_set_operation_parse(xmlTextReaderPtr, char *, oval_setobject_operation_t);
oval_filter_action_t oval_filter_action_parse(xmlTextReaderPtr, char *, oval_filter_action_t);
oval_variable_type_t oval_variable_type_parse(xmlTextReaderPtr);
oval_component_type_t oval_component_type_parse(xmlTextReaderPtr);
oval_arithmetic_operation_t oval_arithmetic_operation_parse(xmlTextReaderPtr, char *, oval_arithmetic_operation_t);
oval_datetime_format_t oval_datetime_format_parse(xmlTextReaderPtr, char *, oval_arithmetic_operation_t);
oval_message_level_t oval_message_level_parse(xmlTextReaderPtr, char *, oval_message_level_t);
void oval_variable_set_type(struct oval_variable *variable, oval_variable_type_t type);

typedef void (*oval_affected_consumer) (struct oval_affected *, void *);
int oval_affected_parse_tag(xmlTextReaderPtr reader,
			    struct oval_parser_context *context, oval_affected_consumer, void *);
void oval_affected_to_print(struct oval_affected *affected, char *indent, int idx);

int oval_test_parse_tag(xmlTextReaderPtr reader, struct oval_parser_context *context);
void oval_test_to_print(struct oval_test *test, char *indent, int index);
xmlNode *oval_test_to_dom(struct oval_test *, xmlDoc *, xmlNode *);

void oval_criteria_node_to_print(struct oval_criteria_node *node, char *indent, int index);
xmlNode *oval_criteria_node_to_dom(struct oval_criteria_node *, xmlDoc *, xmlNode *);
typedef void (*oval_criteria_consumer) (struct oval_criteria_node *, void *);
int oval_criteria_parse_tag(xmlTextReaderPtr, struct oval_parser_context *, oval_criteria_consumer, void *);

typedef void (*oval_reference_consumer) (struct oval_reference *, void *);
int oval_reference_parse_tag(xmlTextReaderPtr, struct oval_parser_context *, oval_reference_consumer, void *);
void oval_reference_to_print(struct oval_reference *ref, char *indent, int index);

int oval_definition_parse_tag(xmlTextReaderPtr reader, struct oval_parser_context *context);
void oval_definition_to_print(struct oval_definition *definition, char *indent, int idx);
xmlNode *oval_definition_to_dom(struct oval_definition *, xmlDoc *, xmlNode *);

int oval_object_parse_tag(xmlTextReaderPtr reader, struct oval_parser_context *context);
void oval_object_to_print(struct oval_object *object, char *indent, int idx);
xmlNode *oval_object_to_dom(struct oval_object *, xmlDoc *, xmlNode *);

int oval_state_parse_tag(xmlTextReaderPtr reader, struct oval_parser_context *context);
void oval_state_to_print(struct oval_state *, char *indent, int index);
xmlNode *oval_state_to_dom(struct oval_state *, xmlDoc *, xmlNode *);

int oval_variable_parse_tag(xmlTextReaderPtr reader, struct oval_parser_context *context);
void oval_variable_to_print(struct oval_variable *variable, char *indent, int idx);
xmlNode *oval_variable_to_dom(struct oval_variable *, xmlDoc *, xmlNode *);

void oval_variable_binding_to_dom(struct oval_variable_binding *, xmlDoc *, xmlNode *);
typedef void (*oval_variable_binding_consumer) (struct oval_variable_binding *, void *);
int oval_variable_binding_parse_tag
    (xmlTextReaderPtr, struct oval_parser_context *, oval_variable_binding_consumer, void *);

const char *oval_variable_type_get_text(oval_variable_type_t);
const char *oval_filter_action_get_text(oval_filter_action_t);

struct oval_filter;
struct oval_filter *oval_filter_new(struct oval_definition_model *);
void oval_filter_free(struct oval_filter *);
struct oval_filter *oval_filter_clone(struct oval_definition_model *, struct oval_filter *);
struct oval_state *oval_filter_get_state(struct oval_filter *);
oval_filter_action_t oval_filter_get_filter_action(struct oval_filter *);
bool oval_filter_is_locked(struct oval_filter *);
bool oval_filter_is_valid(struct oval_filter *);
void oval_filter_set_state(struct oval_filter *, struct oval_state *);
void oval_filter_set_filter_action(struct oval_filter *, oval_filter_action_t );
typedef void (*oval_filter_consumer) (struct oval_filter *, void *);
int oval_filter_parse_tag(xmlTextReaderPtr, struct oval_parser_context *, oval_filter_consumer, void *);

typedef void (*oval_object_content_consumer) (struct oval_object_content *, void *);
xmlNode *oval_object_content_to_dom(struct oval_object_content *, xmlDoc *, xmlNode *);
int oval_object_content_parse_tag(xmlTextReaderPtr, struct oval_parser_context *, oval_object_content_consumer, void *);
void oval_object_content_to_print(struct oval_object_content *, char *indent, int index);
struct oval_filter *oval_object_content_get_filter(struct oval_object_content *);
void oval_object_content_set_filter(struct oval_object_content *, struct oval_filter *);

int oval_state_content_parse_tag(xmlTextReaderPtr, struct oval_parser_context *, oscap_consumer_func, void *);
xmlNode *oval_state_content_to_dom(struct oval_state_content *, xmlDoc *, xmlNode *);

typedef void (*oval_behavior_consumer) (struct oval_behavior *, void *);
int oval_behavior_parse_tag(xmlTextReaderPtr, struct oval_parser_context *,
			    oval_family_t, oval_behavior_consumer, void *);
void oval_behavior_to_print(struct oval_behavior *, char *indent, int index);

int oval_entity_parse_tag(xmlTextReaderPtr, struct oval_parser_context *, oscap_consumer_func, void *);
void oval_entity_to_print(struct oval_entity *, char *indent, int index);
xmlNode *oval_entity_to_dom(struct oval_entity *, xmlDoc *, xmlNode *);

typedef void (*oval_set_consumer) (struct oval_setobject *, void *);
int oval_set_parse_tag(xmlTextReaderPtr, struct oval_parser_context *, oval_set_consumer, void *);
void oval_set_to_print(struct oval_setobject *, char *indent, int index);
xmlNode *oval_set_to_dom(struct oval_setobject *, xmlDoc *, xmlNode *);

typedef void (*oval_value_consumer) (struct oval_value *, void *);
int oval_value_parse_tag(xmlTextReaderPtr, struct oval_parser_context *, oval_value_consumer, void *);
void oval_value_to_print(struct oval_value *value, char *indent, int idx);
xmlNode *oval_value_to_dom(struct oval_value *, xmlDoc *, xmlNode *);
int oval_value_cast(struct oval_value *value, oval_datatype_t new_dt);

oval_syschar_collection_flag_t oval_component_compute(struct oval_syschar_model *sysmod, struct oval_component *component,
						      struct oval_collection *value_collection);
oval_syschar_collection_flag_t oval_component_query(oval_probe_session_t *sess, struct oval_component *component,
						    struct oval_collection *value_collection);
int oval_probe_session_query_object(oval_probe_session_t *sess, struct oval_object *object);
typedef void (*oval_component_consumer) (struct oval_component *, void *);
int oval_component_parse_tag(xmlTextReaderPtr, struct oval_parser_context *, oval_component_consumer, void *);
void oval_component_to_print(struct oval_component *, char *indent, int index);
xmlNode *oval_component_to_dom(struct oval_component *, xmlDoc *, xmlNode *);

typedef void (*oval_message_consumer) (struct oval_message *, void *);
int oval_message_parse_tag(xmlTextReaderPtr, struct oval_parser_context *, oscap_consumer_func, void *);
void oval_message_to_print(struct oval_message *message, char *indent, int index);
void oval_message_to_dom(struct oval_message *, xmlDoc *, xmlNode *);

void oval_definition_model_add_definition(struct oval_definition_model *, struct oval_definition *);
void oval_definition_model_add_test(struct oval_definition_model *, struct oval_test *);
void oval_definition_model_add_object(struct oval_definition_model *, struct oval_object *);
void oval_definition_model_add_state(struct oval_definition_model *, struct oval_state *);
void oval_definition_model_add_variable(struct oval_definition_model *, struct oval_variable *);

//Synthetic object subtype for probing system info.
#define  OVAL_INDEPENDENT_SYSCHAR_SUBTYPE 7999

OSCAP_HIDDEN_END
#endif

/**
 * @file   probe-api.h
 * @brief  Probe API public header
 * @author "Daniel Kopecek" <dkopecek@redhat.com>
 * @author "Tomas Heinrich" <theinric@redhat.com>
 *
 * @addtogroup PROBEAPI
 * This file contains functions for manipulating with the S-exp representation
 * of OVAL objects and items. Currently object and items have the same structure
 * and the API distinction is just formal. However, the structure can diverge in
 * the future and the API functions for manipulating with items should be used
 * only with items and vice versa. The most recent description of the object and
 * item structure can be found in this file and should be used as the main source
 * of information for implementing new API functions. In the following text, the
 * term `element' referers to the general structure which is used to represent
 * the various components of an OVAL document, particularly the OVAL objects,
 * items and entities.
 *
 * \paragraph Element structure
 * The basic structure of an element looks like this:\n
 * \n
 * (foo_object bar)\n
 * \n
 * \em foo_object is the element name and \em bar is the value of the element. There
 * can be 0 to n values. In case the element has some attributes set the structure
 * changes to:\n
 * \n
 * ((foo_object :attr1 val1) bar)\n
 * \n
 * where \em attr1 is the name of the attribute and \em val1 is the attribute's value.
 * The colon in the attribute name signals that the attribute has a value.
 *
 * @{
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
 *      Daniel Kopecek <dkopecek@redhat.com>
 */

#pragma once
#ifndef PROBE_API_H
#define PROBE_API_H

#include <seap.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <oval_definitions.h>
#include <oval_system_characteristics.h>
#include <oval_results.h>

/*
 * items
 */

/**
 * Build a new item according to the scpecified format.
 * @param fmt the desired format
 */
SEXP_t *probe_item_build(const char *fmt, ...);

/* SEXP_t *probe_item_creat (const char *name, SEXP_t *attrs, ...); */
/**
 * Create a new item consisting of a name, optional attributes argument and an arbitrary number of entities.
 * Every entity is a triple:
 * const char *name - name of the new entity
 * SEXP_t attrs - optional list of entity attributes in a sexp, can be NULL
 * SEXP_t val - value of the new entity in a sexp
 * This function increments its SEXP_t arguments' reference count.
 * @param name mandatory name argument
 * @param attrs optional item's attributes argument
 * @param ... arbitrary number of entity arguments
 */
SEXP_t *probe_item_creat(const char *name, SEXP_t * attrs, ...);

/**
 * Create a new item with just a name and optional attributes argument.
 * @param name item's name
 * @param attrs optional attributes argument
 */
SEXP_t *probe_item_new(const char *name, SEXP_t * attrs);

/**
 * Add a new attribute to an item.
 * This function increments its val argument's reference count.
 * @param item the item to be modified
 * @param name name of the new attribute
 * @param val value of the new attribute
 */
SEXP_t *probe_item_attr_add(SEXP_t * item, const char *name, SEXP_t * val);

/**
 * Add a new entity to an item.
 * This function increments its attrs and val arguments' reference count.
 * @param item the item to be modified
 * @param name name of the new entity
 * @param attrs optional attributes of the new entity
 * @param val value of the new entity
 */
SEXP_t *probe_item_ent_add(SEXP_t * item, const char *name, SEXP_t * attrs, SEXP_t * val);

/**
 * Set item's status.
 * @param obj the item to be modified
 * @param status the new status
 */
int probe_item_setstatus(SEXP_t * obj, oval_syschar_status_t status);

/**
 * Set status of an item's entity.
 * @param obj the item to be modified
 * @param name the name of the entity
 * @param n select the n-th occurence of an entity with the specified name
 * @param status the new status
 */
int probe_itement_setstatus(SEXP_t * obj, const char *name, uint32_t n, oval_syschar_status_t status);

/**
 *
 */
struct id_desc_t;

/**
 * Get a new unique id (within a probe) for an item.
 * @param id_desc pointer to a structure holding the global id context
 * @return a new id
 */
SEXP_t *probe_item_newid(struct id_desc_t *id_desc);

/**
 * Reset the item id generator.
 * @param id_desc pointer to a structure holding the global id context
 */
void probe_item_resetidctr(struct id_desc_t *id_desc);

#define probe_item_getent(item, name, n) probe_obj_getent (item, name, n)

/*
 * attributes
 */

/**
 * Create a new list of attributes.
 * This function increments its val argument's reference count.
 * @param name the name of the attribute
 * @param val the value of the attribute
 * @param ... there can be an arbitrary number of name - value pairs
 */
SEXP_t *probe_attr_creat(const char *name, const SEXP_t * val, ...);

/*
 * objects
 */

/**
 * Build a new object according to the scpecified format.
 * @param fmt the desired format
 */
SEXP_t *probe_obj_build(const char *fmt, ...);

/**
 * Create a new object consisting of a name, optional attributes argument and an arbitrary number of entities.
 * Every entity is a triple:
 * const char *name - name of the new entity
 * SEXP_t attrs - optional list of entity attributes in a sexp, can be NULL
 * SEXP_t val - value of the new entity in a sexp
 * This function increments its SEXP_t arguments' reference count.
 * @param name mandatory name argument
 * @param attrs optional object's attributes argument
 * @param ... arbitrary number of entity arguments
 */
SEXP_t *probe_obj_creat(const char *name, SEXP_t * attrs, ...);

/**
 * Create a new object with just a name and optional attributes argument.
 * This function increments its SEXP_t argument's reference count.
 * @param name object's name
 * @param attrs optional attributes argument
 */
SEXP_t *probe_obj_new(const char *name, SEXP_t * attrs);

/**
 * Get an entity from an object.
 * @param obj the queried object
 * @param name the name of the entity
 * @param n select the n-th occurence of an entity with the specified name
 */
SEXP_t *probe_obj_getent(const SEXP_t * obj, const char *name, uint32_t n);

/**
 * Get the value of an object's entity.
 * The function respects the var_ref attribute and returns the currently selected value.
 * @param obj the queried object
 * @param name the name of the entity
 * @param n select the n-th occurence of an entity with the specified name
 */
SEXP_t *probe_obj_getentval(const SEXP_t * obj, const char *name, uint32_t n);

/**
 * Get the list of values of an object's entity.
 * If the entity uses var_ref, there can be more than one value.
 * @param obj the queried object
 * @param name the name of the entity
 * @param n select the n-th occurence of an entity with the specified name
 * @param res the resulting value list is stored in this argument
 * @return number of values in the list stored in the res argument
 */
int probe_obj_getentvals(const SEXP_t * obj, const char *name, uint32_t n, SEXP_t ** res);

/**
 * Get the value of an object's attribute.
 * @param obj the queried object
 * @param name the name of the attribute
 */
SEXP_t *probe_obj_getattrval(const SEXP_t * obj, const char *name);

/**
 * Check whether the specified attribute exists.
 * @param obj the queried object
 * @param name the name of the attribute
 */
bool probe_obj_attrexists(const SEXP_t * obj, const char *name);

/**
 * Set objects's status.
 * @param obj the object to be modified
 * @param status the new status
 */
int probe_obj_setstatus(SEXP_t * obj, oval_syschar_status_t status);

/**
 * Set status of an object's entity.
 * @param obj the object to be modified
 * @param name the name of the entity
 * @param n select the n-th occurence of an entity with the specified name
 * @param status the new status
 */
int probe_objent_setstatus(SEXP_t * obj, const char *name, uint32_t n, oval_syschar_status_t status);

/**
 * Get the name of an object.
 * @param obj the queried object
 */
char *probe_obj_getname(const SEXP_t * obj);

/**
 * Get the name of an object.
 * The name is stored in the provided buffer.
 * @param obj the queried object
 * @param buffer the buffer to store the name in
 * @param buflen the length of the buffer
 */
size_t probe_obj_getname_r(const SEXP_t * obj, char *buffer, size_t buflen);

/*
 * collected objects
 */

SEXP_t *_probe_cobj_new(oval_syschar_collection_flag_t flag, const SEXP_t *item_list);
SEXP_t *_probe_cobj_get_items(const SEXP_t *cobj);
oval_syschar_collection_flag_t _probe_cobj_get_flag(const SEXP_t *cobj);
oval_syschar_collection_flag_t _probe_cobj_combine_flags(oval_syschar_collection_flag_t f1,
							 oval_syschar_collection_flag_t f2,
							 oval_setobject_operation_t op);

/*
 * entities
 */

/**
 * Create a new list of entities.
 * This function increments its SEXP_t arguments' reference count.
 * @param name the name of the entity
 * @param attrs optional entity's attributes argument
 * @param val the value of the entity
 * @param ... there can be an arbitrary number of name - attributes - value triples
 */
SEXP_t *probe_ent_creat(const char *name, SEXP_t * attrs, SEXP_t * val, ...);

/**
 * Create a new entity.
 * This function increments its SEXP_t arguments' reference count.
 * @param name the name of the entity
 * @param attrs optional entity's attributes argument
 * @param val the value of the entity
 */
SEXP_t *probe_ent_creat1(const char *name, SEXP_t * attrs, SEXP_t * val);

/**
 * Add a new attribute to an entity.
 * This function increments its val argument's reference count.
 * @param ent the entity to be modified
 * @param name name of the new attribute
 * @param val value of the new attribute
 */
SEXP_t *probe_ent_attr_add(SEXP_t * ent, const char *name, SEXP_t * val);

/**
 * Get the value of an entity.
 * The function respects the var_ref attribute and returns the currently selected value.
 * @param ent the queried entity
 */
SEXP_t *probe_ent_getval(const SEXP_t * ent);

/**
 * Get the list of values of an entity.
 * If the entity uses var_ref, there can be more than one value.
 * @param ent the queried entity
 * @param res the resulting value list is stored in this argument
 * @return number of values in the list stored in the res argument
 */
int probe_ent_getvals(const SEXP_t * ent, SEXP_t ** res);

/**
 * Get the value of an entity's attribute.
 * @param ent the queried entity
 * @param name the name of the attribute
 */
SEXP_t *probe_ent_getattrval(const SEXP_t * ent, const char *name);

/**
 * Check whether the specified attribute exists.
 * @param ent the queried entity
 * @param name the name of the attribute
 */
bool probe_ent_attrexists(const SEXP_t * ent, const char *name);

/**
 * Set the OVAL data type of an entity.
 * @param ent the queried entity
 * @param type the new data type
 */
int probe_ent_setdatatype(SEXP_t * ent, oval_datatype_t type);

/**
 * Get the OVAL data type of an entity.
 * @param ent the queried entity
 */
oval_datatype_t probe_ent_getdatatype(const SEXP_t * ent);

/**
 * Set entity's mask.
 * @param ent the queried entity
 * @mask the new mask
 */
int probe_ent_setmask(SEXP_t * ent, bool mask);

/**
 * Get entity's mask.
 * @param ent the queried entity
 */
bool probe_ent_getmask(const SEXP_t * ent);

/**
 * Set entity's status.
 * @param ent the entity to be modified
 * @param status the new status
 */
int probe_ent_setstatus(SEXP_t * ent, oval_syschar_status_t status);

/**
 * Get entity status.
 * @param ent the queried entity
 */
oval_syschar_status_t probe_ent_getstatus(const SEXP_t * ent);

/**
 * Get the name of an entity.
 * @param ent the queried entity
 */
char *probe_ent_getname(const SEXP_t * ent);

/**
 * Get the name of an entity.
 * The name is stored in the provided buffer.
 * @param ent the queried entity
 * @param buffer the buffer to store the name in
 * @param buflen the length of the buffer
 */
size_t probe_ent_getname_r(const SEXP_t * ent, char *buffer, size_t buflen);

/**
 * Free the memory allocated by the probe_* functions.
 * @param obj the object to be freed
 */
void probe_free(SEXP_t * obj);

#define PROBE_EINVAL     1	/**< Invalid type/value/format */
#define PROBE_ENOELM     2	/**< Missing element OBSOLETE: use ENOENT */
#define PROBE_ENOVAL     3	/**< Missing value */
#define PROBE_ENOATTR    4	/**< Missing attribute */
#define PROBE_EINIT      5	/**< Initialization failed */
#define PROBE_ENOMEM     6	/**< No memory */
#define PROBE_EOPNOTSUPP 7	/**< Not supported */
#define PROBE_ERANGE     8	/**< Out of range */
#define PROBE_EDOM       9	/**< Out of domain */
#define PROBE_EFAULT    10	/**< Memory fault/NULL value */
#define PROBE_EACCES    11	/**< Operation not perimitted */
#define PROBE_ESETEVAL  12	/**< Set evaluation failed */
#define PROBE_ENOENT    13      /**< Missing entity */
#define PROBE_EFATAL   254	/**< Unrecoverable error */
#define PROBE_EUNKNOWN 255	/**< Unknown/Unexpected error */

#define OVAL_STATUS_UNKNOWN      SYSCHAR_STATUS_UNKNOWN
#define OVAL_STATUS_ERROR        SYSCHAR_STATUS_ERROR
#define OVAL_STATUS_EXISTS       SYSCHAR_STATUS_EXISTS
#define OVAL_STATUS_DOESNOTEXIST SYSCHAR_STATUS_DOES_NOT_EXIST
#define OVAL_STATUS_NOTCOLLECTED SYSCHAR_STATUS_NOT_COLLECTED

#define PROBECMD_STE_FETCH 1 /**< State fetch command code */
#define PROBECMD_OBJ_EVAL  2 /**< Object eval command code */
#define PROBECMD_RESET     3 /**< Reset command code */

void *probe_init(void) __attribute__ ((unused));
void probe_fini(void *) __attribute__ ((unused));
SEXP_t *probe_main(SEXP_t *, int *, void *) __attribute__ ((nonnull(1, 2)));

#define PROBE_VARREF_HANDLING 0
#define PROBE_RESULT_CACHING  1

int probe_setoption(int option, ...);

SEXP_t *probe_prepare_filters(SEXP_t *obj);
bool probe_item_filtered(SEXP_t *item, SEXP_t *filters);

int probe_result_additem(SEXP_t *result, SEXP_t *item);

#endif				/* PROBE_API_H */

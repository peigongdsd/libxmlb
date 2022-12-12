/*
 * Copyright (C) 2018 Richard Hughes <richard@hughsie.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#pragma once

#include <string.h>

#include "xb-opcode.h"

G_BEGIN_DECLS

/* maximum number of tokens supported for each element -- this is a compromise
 * between making the _XbOpcode struct too large and search results */
#define XB_OPCODE_TOKEN_MAX 32

struct _XbOpcode {
	XbOpcodeKind kind;
	guint32 val;
	gpointer ptr;
	guint8 tokens_len;
	const gchar *tokens[XB_OPCODE_TOKEN_MAX + 1];
	GDestroyNotify destroy_func;
	guint8 level;
};

#define XB_OPCODE_INIT()                                                                           \
	{                                                                                          \
		0, 0, NULL, 0, {NULL}, NULL, 0                                                     \
	}

/**
 * xb_opcode_steal:
 * @op_ptr: (transfer full): pointer to an #XbOpcode to steal
 *
 * Steal the stack-allocated #XbOpcode pointed to by @op_ptr, returning its
 * value and clearing its previous storage location using `memset()`.
 *
 * Returns: the value of @op_ptr
 * Since: 0.2.0
 */
static inline XbOpcode
xb_opcode_steal(XbOpcode *op_ptr)
{
	XbOpcode op = *op_ptr;
	memset(op_ptr, 0, sizeof(XbOpcode));
	return op;
}

void
xb_opcode_init(XbOpcode *opcode,
	       XbOpcodeKind kind,
	       const gchar *str,
	       guint32 val,
	       GDestroyNotify destroy_func);
void
xb_opcode_clear(XbOpcode *opcode);
void
xb_opcode_bind_init(XbOpcode *opcode);
gboolean
xb_opcode_is_binding(XbOpcode *self);
G_DEPRECATED_FOR(xb_value_bindings_bind_str)
void
xb_opcode_bind_str(XbOpcode *self, gchar *str, GDestroyNotify destroy_func);
G_DEPRECATED_FOR(xb_value_bindings_bind_val)
void
xb_opcode_bind_val(XbOpcode *self, guint32 val);
void
xb_opcode_set_kind(XbOpcode *self, XbOpcodeKind kind);
void
xb_opcode_set_val(XbOpcode *self, guint32 val);
gboolean
xb_opcode_append_token(XbOpcode *self, const gchar *val);
const gchar **
xb_opcode_get_tokens(XbOpcode *self);
gchar *
xb_opcode_get_sig(XbOpcode *self);
void
xb_opcode_bool_init(XbOpcode *opcode, gboolean val);
gboolean
xb_opcode_has_flag(XbOpcode *self, XbOpcodeFlags flag);
void
xb_opcode_add_flag(XbOpcode *self, XbOpcodeFlags flag);

void
xb_opcode_set_level(XbOpcode *self, guint8 level);
guint8
xb_opcode_get_level(XbOpcode *self);

G_DEFINE_AUTO_CLEANUP_CLEAR_FUNC(XbOpcode, xb_opcode_clear)

G_END_DECLS

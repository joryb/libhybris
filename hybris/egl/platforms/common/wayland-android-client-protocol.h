/* 
 * Copyright © 2012 Collabora, Ltd.
 * 
 * Permission to use, copy, modify, distribute, and sell this
 * software and its documentation for any purpose is hereby granted
 * without fee, provided that the above copyright notice appear in
 * all copies and that both that copyright notice and this permission
 * notice appear in supporting documentation, and that the name of
 * the copyright holders not be used in advertising or publicity
 * pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied
 * warranty.
 * 
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 */

#ifndef ANDROID_CLIENT_PROTOCOL_H
#define ANDROID_CLIENT_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-client.h"

struct wl_client;
struct wl_resource;

struct android_wlegl;
struct android_wlegl_handle;
struct android_wlegl_server_buffer_handle;

extern const struct wl_interface android_wlegl_interface;
extern const struct wl_interface android_wlegl_handle_interface;
extern const struct wl_interface android_wlegl_server_buffer_handle_interface;

#ifndef ANDROID_WLEGL_ERROR_ENUM
#define ANDROID_WLEGL_ERROR_ENUM
enum android_wlegl_error {
	ANDROID_WLEGL_ERROR_BAD_HANDLE = 0,
	ANDROID_WLEGL_ERROR_BAD_VALUE = 1,
};
#endif /* ANDROID_WLEGL_ERROR_ENUM */

#define ANDROID_WLEGL_CREATE_HANDLE	0
#define ANDROID_WLEGL_CREATE_BUFFER	1
#define ANDROID_WLEGL_GET_SERVER_BUFFER_HANDLE	2

static inline void
android_wlegl_set_user_data(struct android_wlegl *android_wlegl, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) android_wlegl, user_data);
}

static inline void *
android_wlegl_get_user_data(struct android_wlegl *android_wlegl)
{
	return wl_proxy_get_user_data((struct wl_proxy *) android_wlegl);
}

static inline void
android_wlegl_destroy(struct android_wlegl *android_wlegl)
{
	wl_proxy_destroy((struct wl_proxy *) android_wlegl);
}

static inline struct android_wlegl_handle *
android_wlegl_create_handle(struct android_wlegl *android_wlegl, int32_t num_fds, struct wl_array *ints)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) android_wlegl,
			 ANDROID_WLEGL_CREATE_HANDLE, &android_wlegl_handle_interface, NULL, num_fds, ints);

	return (struct android_wlegl_handle *) id;
}

static inline struct wl_buffer *
android_wlegl_create_buffer(struct android_wlegl *android_wlegl, int32_t width, int32_t height, int32_t stride, int32_t format, int32_t usage, struct android_wlegl_handle *native_handle)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) android_wlegl,
			 ANDROID_WLEGL_CREATE_BUFFER, &wl_buffer_interface, NULL, width, height, stride, format, usage, native_handle);

	return (struct wl_buffer *) id;
}

static inline struct android_wlegl_server_buffer_handle *
android_wlegl_get_server_buffer_handle(struct android_wlegl *android_wlegl, int32_t width, int32_t height, int32_t format, int32_t usage)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) android_wlegl,
			 ANDROID_WLEGL_GET_SERVER_BUFFER_HANDLE, &android_wlegl_server_buffer_handle_interface, NULL, width, height, format, usage);

	return (struct android_wlegl_server_buffer_handle *) id;
}

#ifndef ANDROID_WLEGL_HANDLE_ERROR_ENUM
#define ANDROID_WLEGL_HANDLE_ERROR_ENUM
enum android_wlegl_handle_error {
	ANDROID_WLEGL_HANDLE_ERROR_TOO_MANY_FDS = 0,
};
#endif /* ANDROID_WLEGL_HANDLE_ERROR_ENUM */

#define ANDROID_WLEGL_HANDLE_ADD_FD	0
#define ANDROID_WLEGL_HANDLE_DESTROY	1

static inline void
android_wlegl_handle_set_user_data(struct android_wlegl_handle *android_wlegl_handle, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) android_wlegl_handle, user_data);
}

static inline void *
android_wlegl_handle_get_user_data(struct android_wlegl_handle *android_wlegl_handle)
{
	return wl_proxy_get_user_data((struct wl_proxy *) android_wlegl_handle);
}

static inline void
android_wlegl_handle_add_fd(struct android_wlegl_handle *android_wlegl_handle, int32_t fd)
{
	wl_proxy_marshal((struct wl_proxy *) android_wlegl_handle,
			 ANDROID_WLEGL_HANDLE_ADD_FD, fd);
}

static inline void
android_wlegl_handle_destroy(struct android_wlegl_handle *android_wlegl_handle)
{
	wl_proxy_marshal((struct wl_proxy *) android_wlegl_handle,
			 ANDROID_WLEGL_HANDLE_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) android_wlegl_handle);
}

/**
 * android_wlegl_server_buffer_handle - a server allocated buffer
 * @buffer_fd: (none)
 * @buffer_ints: (none)
 * @buffer: the wl_buffer
 *
 * On creation a server_side_buffer object will immediately send the
 * "buffer_fd" and "buffer_ints" events needed by the client to reference
 * the gralloc buffer, followed by the "buffer" event carrying the
 * wl_buffer object.
 */
struct android_wlegl_server_buffer_handle_listener {
	/**
	 * buffer_fd - (none)
	 * @fd: (none)
	 */
	void (*buffer_fd)(void *data,
			  struct android_wlegl_server_buffer_handle *android_wlegl_server_buffer_handle,
			  int32_t fd);
	/**
	 * buffer_ints - (none)
	 * @ints: (none)
	 */
	void (*buffer_ints)(void *data,
			    struct android_wlegl_server_buffer_handle *android_wlegl_server_buffer_handle,
			    struct wl_array *ints);
	/**
	 * buffer - the wl_buffer
	 * @buffer: (none)
	 * @format: (none)
	 * @stride: (none)
	 *
	 * This event will be sent after the ints and all the fds have
	 * been sent
	 */
	void (*buffer)(void *data,
		       struct android_wlegl_server_buffer_handle *android_wlegl_server_buffer_handle,
		       struct wl_buffer *buffer,
		       int32_t format,
		       int32_t stride);
};

static inline int
android_wlegl_server_buffer_handle_add_listener(struct android_wlegl_server_buffer_handle *android_wlegl_server_buffer_handle,
						const struct android_wlegl_server_buffer_handle_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) android_wlegl_server_buffer_handle,
				     (void (**)(void)) listener, data);
}

static inline void
android_wlegl_server_buffer_handle_set_user_data(struct android_wlegl_server_buffer_handle *android_wlegl_server_buffer_handle, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) android_wlegl_server_buffer_handle, user_data);
}

static inline void *
android_wlegl_server_buffer_handle_get_user_data(struct android_wlegl_server_buffer_handle *android_wlegl_server_buffer_handle)
{
	return wl_proxy_get_user_data((struct wl_proxy *) android_wlegl_server_buffer_handle);
}

static inline void
android_wlegl_server_buffer_handle_destroy(struct android_wlegl_server_buffer_handle *android_wlegl_server_buffer_handle)
{
	wl_proxy_destroy((struct wl_proxy *) android_wlegl_server_buffer_handle);
}

#ifdef  __cplusplus
}
#endif

#endif

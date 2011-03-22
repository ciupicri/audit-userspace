/* queue.h -- a queue abstraction
 * Copyright 2009, 2011 Red Hat Inc., Durham, North Carolina.
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
 *      Steve Grubb <sgrubb@redhat.com>
 *      Miloslav Trmač <mitr@redhat.com>
 */

#ifndef QUEUE_HEADER
#define QUEUE_HEADER

#include <sys/types.h>
#include "libaudit.h"
#include "remote-config.h"

int init_queue(remote_conf_t *config);
int enqueue(const char *data);
int peek_queue(char *buf, size_t);
void dequeue(void);
int queue_length(void);
void destroy_queue(void);

 /* The new interface */

struct queue;

enum {
	/* Queue storage.  Both options can be set at the same time. */
	Q_IN_MEMORY = 1 << 0,	/* Keep a copy of the queue in memory */
	Q_IN_FILE = 1 << 1,	/* Store the queue in a file */
	/* Other flags */
	/* With Q_IN_FILE, create the queue if it does not exist */
	Q_CREAT = 1 << 2,
	Q_EXCL = 1 << 3,	/* With Q_CREAT, don't open an existing queue */
	Q_SYNC = 1 << 4, /* With Q_IN_FILE, fdatasync() after each operation */
	/* With Q_IN_FILE, resize the queue length if necessary */
	Q_RESIZE = 1 << 5,
};

/* Open a queue using Q_FLAGS and return it.

   If Q_IN_FILE, use PATH for the file.
   If Q_IN_FILE, NUM_ENTRIES must be the same for all users of the file unless
   Q_RESIZE is set.
   ENTRY_SIZE is the maximum length of a stored string, including the trailing
   NUL.  If Q_IN_FILE, it must be the same for all users of the file.

   On error, return NULL and set errno.

   Note that the returned queue may not be concurrently accessed by more than
   one thread. */
struct queue *q_open(int q_flags, const char *path, size_t num_entries,
		     size_t entry_size);
/* Close Q. */
void q_close(struct queue *q);

/* Add DATA to tail of Q and return 0.
   On error, return -1 and set errno. */
int q_append(struct queue *q, const char *data);
/* Peek at head of Q, storing it into BUF of SIZE.
   Return 1 if an entry exists, 0 if queue is empty.
   On error, return -1 and set errno. */
int q_peek(struct queue *q, char *buf, size_t size);
/* Drop head of Q and return 0.
   On error, return -1 and set errno. */
int q_drop_head(struct queue *q);
/* Return the number of entires in Q. */
size_t q_queue_length(struct queue *q);

#endif


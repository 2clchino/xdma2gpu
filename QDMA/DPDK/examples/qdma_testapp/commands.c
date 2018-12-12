/*-
 *   BSD LICENSE
 *
 *   Copyright(c) 2017-2018 Xilinx, Inc. All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*-
 *   BSD LICENSE
 *
 *   Copyright(c) 2010-2014 Intel Corporation. All rights reserved.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright (c) 2009, Olivier MATZ <zer0@droids-corp.org>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <netinet/in.h>
#include <termios.h>
#ifndef __linux__
#include <net/socket.h>
#endif

#include <cmdline_rdline.h>
#include <cmdline_parse.h>
#include <cmdline_parse_ipaddr.h>
#include <cmdline_parse_num.h>
#include <cmdline_parse_string.h>
#include <cmdline.h>

#include <rte_string_fns.h>
#include <rte_ethdev.h>
#include <fcntl.h>

#include "parse_obj_list.h"
#include "pcierw.h"
#include "commands.h"
#include "qdma_regs.h"
#include "testapp.h"
#include "../../drivers/net/qdma/qdma.h"

/* Command help */
struct cmd_help_result {
	cmdline_fixed_string_t help;
};

static void cmd_help_parsed(__attribute__((unused)) void *parsed_result,
						struct cmdline *cl,
					__attribute__((unused)) void *data)
{
	cmdline_printf(cl,
			"Demo example of command line interface in RTE\n\n"
			"This is a readline-like interface that can be "
			"used to\n debug your RTE application.\n\n"
			"Supported Commands:\n"
			"\tport_init            <port-id> <queue-id base> "
						"<num-queues> <st-queues> "
						"<ring-depth> <pkt-buff-size> "
					    	"    :port-initailization\n"
			"\tport_close           <port-id> "
			":port-close\n"
			"\treg_read             <port-id> <bar-num> <address> "
			":Reads Specified Register\n"
			"\treg_write            <port-id> <bar-num> <address> "
						"<value>    "
			":Writes Specified Register\n"
			"\tdma_to_device        <port-id> <num-queues> "
						"<input-filename> "
			"<dst_addr> <size> <iterations>   "
			":To Transmit\n"
			"\tdma_from_device      <port-id> <num-queues> "
						"<output-filename> "
			"<src_addr> <size> <iterations>  "
			":To Receive\n"
			"\treg_dump             <port-id>  "
			":To dump all the valid registers\n"
			"\tqueue_dump           <port-id> <queue-id>  "
			":To dump the queue-context of a queue-number\n"
			"\tdesc_dump            <port-id> <queue-id>  "
			":To dump the descriptor-fields of a "
			"queue-number\n"
			"\tload_cmds            <file-name> "
			":To execute the list of commands from file\n"
			"\thelp\n"
			"\tCtrl-d                           "
			": To quit from this command-line type Ctrl+d\n"
			"\n");
}

cmdline_parse_token_string_t cmd_help_help =
	TOKEN_STRING_INITIALIZER(struct cmd_help_result, help, "help");

cmdline_parse_inst_t cmd_help = {
	.f = cmd_help_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "show help",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_help_help,
		NULL,
	},
};

struct cmd_obj_port_init_result {
	cmdline_fixed_string_t action;
	cmdline_fixed_string_t port_id;
	cmdline_fixed_string_t queue_base;
	cmdline_fixed_string_t num_queues;
	cmdline_fixed_string_t st_queues;
	cmdline_fixed_string_t nb_descs;
	cmdline_fixed_string_t buff_size;
};

static void cmd_obj_port_init_parsed(void *parsed_result,
			       struct cmdline *cl,
			       __attribute__((unused)) void *data)
{
	struct cmd_obj_port_init_result *res = parsed_result;

	cmdline_printf(cl, "Port-init Port:%s, num-queues:%s, st-queues: %s\n",
			res->port_id, res->num_queues, res->st_queues);

	int port_id = atoi(res->port_id);
	if (pinfo[port_id].num_queues) {
		cmdline_printf(cl, "Error: port-id:%d already initialized\n"
				"To re-initialize please close the port\n",
				port_id);
		return;
	}

	int num_queues   = atoi(res->num_queues);
	int queue_base   = atoi(res->queue_base);
	int st_queues   = atoi(res->st_queues);
	int nb_descs	= atoi(res->nb_descs);
	int buff_size	= atoi(res->buff_size);

	if ((num_queues < 1) || (queue_base + num_queues > MAX_NUM_QUEUES)) {
		cmdline_printf(cl, "Error: please enter valid number of queues,"
				"entered: %d max allowed: %d\n",
				num_queues, MAX_NUM_QUEUES);
		return;
	}

	if (port_id >= num_ports) {
		cmdline_printf(cl, "Error: please enter valid port number: "
				"%d\n", port_id);
		return;
	}
	{
		int result = port_init(port_id, queue_base, num_queues,
					st_queues, nb_descs, buff_size);

		if (result < 0)
			cmdline_printf(cl, "Error: Port initialization on "
					"port-id:%d failed\n", port_id);
		else
			cmdline_printf(cl, "Port initialization done "
					"successfully on port-id:%d\n",
					port_id);
	}
}

cmdline_parse_token_string_t cmd_obj_action_port_init =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_port_init_result, action,
					"port_init");
cmdline_parse_token_string_t cmd_obj_port_init_port_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_port_init_result, port_id,
					NULL);
cmdline_parse_token_string_t cmd_obj_port_init_queue_base =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_port_init_result, queue_base,
					NULL);
cmdline_parse_token_string_t cmd_obj_port_init_num_queues =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_port_init_result, num_queues,
					NULL);
cmdline_parse_token_string_t cmd_obj_port_init_st_queues =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_port_init_result, st_queues,
					NULL);
cmdline_parse_token_string_t cmd_obj_port_init_nb_descs =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_port_init_result, nb_descs,
					NULL);
cmdline_parse_token_string_t cmd_obj_port_init_buff_size =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_port_init_result, buff_size,
					NULL);

cmdline_parse_inst_t cmd_obj_port_init = {
	.f = cmd_obj_port_init_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "port_init  port-id queue-id-base num-queues st-queues "
			"queue-ring-size buffer-size",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_obj_action_port_init,
		(void *)&cmd_obj_port_init_port_id,
		(void *)&cmd_obj_port_init_queue_base,
		(void *)&cmd_obj_port_init_num_queues,
		(void *)&cmd_obj_port_init_st_queues,
		(void *)&cmd_obj_port_init_nb_descs,
		(void *)&cmd_obj_port_init_buff_size,
		NULL,
	},

};

/* Command port-close */
struct cmd_obj_port_close_result {
	cmdline_fixed_string_t action;
	cmdline_fixed_string_t port_id;
};

static void cmd_obj_port_close_parsed(void *parsed_result,
			       struct cmdline *cl,
			       __attribute__((unused)) void *data)
{
	struct cmd_obj_port_close_result *res = parsed_result;

	cmdline_printf(cl, "Port-close on Port-id:%s\n", res->port_id);

	int port_id = atoi(res->port_id);
	if (pinfo[port_id].num_queues == 0) {
		cmdline_printf(cl, "Error: port-id:%d already closed\n",
				port_id);
		return;
	}

	if (port_id >= num_ports) {
		cmdline_printf(cl, "Error: please enter valid port "
					"number: %d\n", port_id);
		return;
	}
	{
		port_close(port_id);
		pinfo[port_id].num_queues = 0;
		cmdline_printf(cl, "Port-id:%d closed successfully\n", port_id);
	}
}

cmdline_parse_token_string_t cmd_obj_action_port_close =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_port_close_result, action,
					"port_close");
cmdline_parse_token_string_t cmd_obj_port_close_port_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_port_close_result, port_id,
					NULL);

cmdline_parse_inst_t cmd_obj_port_close = {
	.f = cmd_obj_port_close_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "port_close  port-id ",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_obj_action_port_close,
		(void *)&cmd_obj_port_close_port_id,
		NULL,
	},

};

/* Command Read addr */
struct cmd_obj_reg_read_result {
	cmdline_fixed_string_t action;
	cmdline_fixed_string_t port_id;
	cmdline_fixed_string_t bar_id;
	cmdline_fixed_string_t addr;
};

static void cmd_obj_reg_read_parsed(void *parsed_result,
			       struct cmdline *cl,
			       __attribute__((unused)) void *data)
{
	struct cmd_obj_reg_read_result *res = parsed_result;

	cmdline_printf(cl, "Read Port:%s, BAR-index:%s, Address:%s\n\n",
				res->port_id, res->bar_id, res->addr);

	int addr   = strtol(res->addr, NULL, 16);

	if (addr % 4) {
		cmdline_printf(cl, "ERROR: Read address must aligned to "
					"a 4-byte boundary.\n\n");
	} else {
		int port_id = atoi(res->port_id);
		int bar_id = atoi(res->bar_id);
		if (port_id >= num_ports) {
			cmdline_printf(cl, "Error: port-id:%d not supported\n "
					"Please enter valid port-id\n",
					port_id);
			return;
		}
		int result = PciRead(bar_id, addr, port_id);

		cmdline_printf(cl, "Read (%d:0x%08x) = 0x%08x\n",
				port_id, addr, result);
	}
}

cmdline_parse_token_string_t cmd_obj_action_reg_read =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_reg_read_result, action,
					"reg_read");
cmdline_parse_token_string_t cmd_obj_reg_read_port_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_reg_read_result, port_id, NULL);
cmdline_parse_token_string_t cmd_obj_reg_read_bar_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_reg_read_result, bar_id, NULL);
cmdline_parse_token_string_t cmd_obj_reg_read_addr =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_reg_read_result, addr, NULL);

cmdline_parse_inst_t cmd_obj_reg_read = {
	.f = cmd_obj_reg_read_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "reg_read port-id bar-id address",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_obj_action_reg_read,
		(void *)&cmd_obj_reg_read_port_id,
		(void *)&cmd_obj_reg_read_bar_id,
		(void *)&cmd_obj_reg_read_addr,
		NULL,
	},

};

/* Command Write addr */
struct cmd_obj_reg_write_result {
	cmdline_fixed_string_t action;
	cmdline_fixed_string_t port_id;
	cmdline_fixed_string_t bar_id;
	cmdline_fixed_string_t address;
	cmdline_fixed_string_t value;
};

static void cmd_obj_reg_write_parsed(void *parsed_result,
			       struct cmdline *cl,
			       __attribute__((unused)) void *data)
{
	struct cmd_obj_reg_write_result *res = parsed_result;

	cmdline_printf(cl, "Write Port:%s, Address:%s, Value:%s\n",
			res->port_id, res->address, res->value);

	int bar_id = atoi(res->bar_id);
	int port_id = atoi(res->port_id);
	int addr   = strtol(res->address, NULL, 16);
	if (port_id >= num_ports) {
		cmdline_printf(cl, "Error: port-id:%d not supported\n "
				"Please enter valid port-id\n", port_id);
		return;
	}
	if (addr % 4) {
		cmdline_printf(cl, "ERROR: Write address must aligned to a "
				"4-byte boundary.\n\n");
	} else{
		int value  = strtol(res->value, NULL, 16);
		PciWrite(bar_id, addr, value, port_id);
		int result = PciRead(bar_id, addr, port_id);
		cmdline_printf(cl, "Read (%d:0x%08x) = 0x%08x\n", port_id, addr,
				result);
	}
}

cmdline_parse_token_string_t cmd_obj_action_reg_write =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_reg_write_result, action,
								"reg_write");
cmdline_parse_token_string_t cmd_obj_reg_write_port_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_reg_write_result, port_id,
					NULL);
cmdline_parse_token_string_t cmd_obj_reg_write_bar_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_reg_write_result, bar_id, NULL);
cmdline_parse_token_string_t cmd_obj_reg_write_address =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_reg_write_result, address,
					NULL);
cmdline_parse_token_string_t cmd_obj_reg_write_value =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_reg_write_result, value, NULL);

cmdline_parse_inst_t cmd_obj_reg_write = {
	.f = cmd_obj_reg_write_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "reg_write port-id bar-id address value",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_obj_action_reg_write,
		(void *)&cmd_obj_reg_write_port_id,
		(void *)&cmd_obj_reg_write_bar_id,
		(void *)&cmd_obj_reg_write_address,
		(void *)&cmd_obj_reg_write_value,
		NULL,
	},

};

/* Command do-xmit */
struct cmd_obj_dma_to_device_result {
	cmdline_fixed_string_t action;
	cmdline_fixed_string_t port_id;
	cmdline_fixed_string_t queues;
	cmdline_fixed_string_t filename;
	cmdline_fixed_string_t dst_addr;
	cmdline_fixed_string_t size;
	cmdline_fixed_string_t loops;
};

static void cmd_obj_dma_to_device_parsed(void *parsed_result,
			       struct cmdline *cl,
			       __attribute__((unused)) void *data)
{
	struct cmd_obj_dma_to_device_result *res = parsed_result;
	int i, ifd, tot_num_desc, offset, size, r_size = 0, total_size = 0;
	int ld_size = 0, loop = 0, ret, j, zbyte = 0, user_bar_idx;
	off_t ret_val;
	int port_id = 0, num_queues = 0, input_size = 0, num_loops = 0;
	int dst_addr = 0;
	uint32_t regval = 0;
	unsigned int q_data_size = 0;

	cmdline_printf(cl, "xmit on Port:%s, filename:%s, num-queues:%s\n\n",
				res->port_id, res->filename, res->queues);

	ifd = open(res->filename, O_RDWR);
	if (ifd < 0) {
		cmdline_printf(cl, "Error: Invalid filename: %s\n",
					res->filename);
		return;
	}

	{
		port_id = atoi(res->port_id);
		if (port_id >= num_ports) {
			cmdline_printf(cl, "Error: port-id:%d not supported\n "
						"Please enter valid port-id\n",
						port_id);
			close(ifd);
			return;
		}
		num_queues = atoi(res->queues);
		if ((unsigned int)num_queues > pinfo[port_id].num_queues) {
			cmdline_printf(cl, "Error: num-queues:%d are more than "
					"the configured queues:%d,\n "
					"Please enter valid number of queues\n",
					num_queues, pinfo[port_id].num_queues);
			close(ifd);
			return;
		}
		if (num_queues == 0) {
			cmdline_printf(cl, "Error: Please enter valid number "
						"of queues\n");
			close(ifd);
			return;
		}
		user_bar_idx = pinfo[port_id].user_bar_idx;
		regval = PciRead(user_bar_idx, C2H_CONTROL_REG, port_id);

		input_size = atoi(res->size);
		num_loops = atoi(res->loops);
		dst_addr = atoi(res->dst_addr);

#ifndef PERF_BENCHMARK
		if (dst_addr + input_size > BRAM_SIZE) {
			cmdline_printf(cl, "Error: (dst_addr %d + input size "
					"%d) shall be less than "
					"BRAM_SIZE %d.\n", dst_addr,
					input_size, BRAM_SIZE);
			close(ifd);
			return;
		}
#endif
		/* For zero-byte transfers, HW expects a
		 * buffer of length 4kb and with desc->len as 0.
		 */
		if (input_size == 0) {
			if ((unsigned int)num_queues <=
					pinfo[port_id].st_queues) {
				zbyte = 1;
			} else {
				cmdline_printf(cl, "Error: Zero-length support "
						"is for queues with ST-mode "
						"only\n");
				close(ifd);
				return;
			}
		}

		if (input_size % num_queues) {
			size = input_size / num_queues;
			r_size = input_size % num_queues;
		} else
			size = input_size / num_queues;

		do {
			total_size = input_size;
			/* transmit data on the number of Queues configured
			 * from the input file
			 */
			for (i = 0, j = 0; i < num_queues; i++, j++) {

				if (total_size == 0)
					q_data_size = pinfo[port_id].buff_size;
				else if (total_size == (r_size + size)) {
					q_data_size = total_size;
					total_size -= total_size;
				} else {
					q_data_size = size;
					total_size -= size;
				}

				if (q_data_size >= pinfo[port_id].buff_size) {
					if (q_data_size %
						pinfo[port_id].buff_size) {
						tot_num_desc = (q_data_size /
						pinfo[port_id].buff_size) + 1;
						ld_size = q_data_size %
						pinfo[port_id].buff_size;
					} else
						tot_num_desc = (q_data_size /
						pinfo[port_id].buff_size);
				} else {
					tot_num_desc = 1;
					ld_size = q_data_size %
						    pinfo[port_id].buff_size;
				}

				if (port_id)
					offset = (input_size/num_queues) * j;
				else
					offset = (input_size/num_queues) * i;

				if ((unsigned int)i < (pinfo[port_id].st_queues)
						&& !(regval & ST_LOOPBACK_EN))
					ret_val = lseek(ifd, 0, SEEK_SET);
				else
					ret_val = lseek(ifd, offset, SEEK_SET);

				if (ret_val == (off_t)-1) {
					cmdline_printf(cl, "DMA-to-Device: "
							"lseek func failed\n");
					close(ifd);
					return;
				}

#ifdef PERF_BENCHMARK
				dst_addr = (dst_addr + (i * q_data_size)) %
								BRAM_SIZE;
#else
				dst_addr = dst_addr + (i * q_data_size);
#endif
				ret =
				update_queue_param(&rte_eth_devices[port_id], i,
						     TX_DST_ADDRESS, dst_addr);
				if (ret < 0) {
					close(ifd);
					return;
				}

				cmdline_printf(cl, "DMA-to-Device: with "
						"input-size:%d, ld_size:%d,"
						"tot_num_desc:%d\n",
						input_size,  ld_size,
						tot_num_desc);
				ret = do_xmit(port_id, ifd, i, ld_size,
						tot_num_desc, zbyte);
				if (ret < 0) {
					close(ifd);
					return;
				}
			}
			++loop;
		} while (loop < num_loops);
		close(ifd);
	}
	cmdline_printf(cl, "\n######## DMA transfer to device is completed "
						"successfully #######\n");
}

cmdline_parse_token_string_t cmd_obj_action_dma_to_device =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_to_device_result, action,
							"dma_to_device");
cmdline_parse_token_string_t cmd_obj_dma_to_device_port_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_to_device_result, port_id,
								NULL);
cmdline_parse_token_string_t cmd_obj_dma_to_device_queues =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_to_device_result, queues,
								NULL);
cmdline_parse_token_string_t cmd_obj_dma_to_device_filename =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_to_device_result, filename,
								NULL);
cmdline_parse_token_string_t cmd_obj_dma_to_device_dst_addr =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_to_device_result, dst_addr,
								NULL);
cmdline_parse_token_string_t cmd_obj_dma_to_device_size =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_to_device_result, size,
								NULL);
cmdline_parse_token_string_t cmd_obj_dma_to_device_loops =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_to_device_result, loops,
								NULL);

cmdline_parse_inst_t cmd_obj_dma_to_device = {
	.f = cmd_obj_dma_to_device_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "dma_to_device port-id num-queues filename dst_addr "
			"size loops",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_obj_action_dma_to_device,
		(void *)&cmd_obj_dma_to_device_port_id,
		(void *)&cmd_obj_dma_to_device_queues,
		(void *)&cmd_obj_dma_to_device_filename,
		(void *)&cmd_obj_dma_to_device_dst_addr,
		(void *)&cmd_obj_dma_to_device_size,
		(void *)&cmd_obj_dma_to_device_loops,
		NULL,
	},

};

/* Command do-recv */
struct cmd_obj_dma_from_device_result {
	cmdline_fixed_string_t action;
	cmdline_fixed_string_t port_id;
	cmdline_fixed_string_t queues;
	cmdline_fixed_string_t filename;
	cmdline_fixed_string_t src_addr;
	cmdline_fixed_string_t size;
	cmdline_fixed_string_t loops;
};

static void cmd_obj_dma_from_device_parsed(void *parsed_result,
			       struct cmdline *cl,
			       __attribute__((unused)) void *data)
{
	struct cmd_obj_dma_from_device_result *res = parsed_result;
	int i, ofd, offset, size, total_size, r_size = 0;
	int mm_tdesc, mm_ld_size = 0;
	int loop = 0, ret, j;
	off_t ret_val;
	int port_id = 0, num_queues = 0, input_size = 0, num_loops = 0;
	int src_addr = 0;
	unsigned int q_data_size = 0;

	cmdline_printf(cl, "recv on Port:%s, filename:%s\n",
						res->port_id, res->filename);

	ofd = open(res->filename, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, 0666);
	if (ofd < 0) {
		cmdline_printf(cl, "Error: Invalid filename: %s\n",
							res->filename);
		return;
	}

	{
		port_id = atoi(res->port_id);
		if (port_id >= num_ports) {
			cmdline_printf(cl, "Error: port-id:%d not supported\n "
					"Please enter valid port-id\n",
						port_id);
			close(ofd);
			return;
		}
		num_queues = atoi(res->queues);
		if ((unsigned int)num_queues > pinfo[port_id].num_queues) {
			cmdline_printf(cl, "Error: num-queues:%d are more than "
					"the configured queues:%d,\n"
					"Please enter valid number of queues\n",
					num_queues, pinfo[port_id].num_queues);
			close(ofd);
			return;
		}
		if (num_queues == 0) {
			cmdline_printf(cl, "Error: Please enter valid number "
						"of queues\n");
			close(ofd);
			return;
		}
		input_size = atoi(res->size);
		num_loops = atoi(res->loops);
		src_addr = atoi(res->src_addr);
#ifndef PERF_BENCHMARK
		if (src_addr + input_size > BRAM_SIZE) {
			cmdline_printf(cl, "Error: (src_addr %d + input "
					"size %d) shall be less than "
					"BRAM_SIZE %d.\n", src_addr,
					input_size, BRAM_SIZE);
			close(ofd);
			return;
		}
#endif
		/* Restrict C2H zerobyte support to ST-mode queues*/
		if (input_size == 0) {
			if ((unsigned int)num_queues >
					pinfo[port_id].st_queues) {
				cmdline_printf(cl, "Error: Zero-length support "
					"is for queues with ST-mode only\n");
				close(ofd);
				return;
			}
		}

		if (input_size % num_queues) {
			size = input_size / num_queues;
			r_size = input_size % num_queues;
		} else
			size = input_size / num_queues;

		do {
			total_size = input_size;
			/* Transmit data on the number of Queues configured
			 * from the input file
			 */
			for (i = 0, j = 0; i < num_queues; i++, j++) {

				if (total_size == (r_size + size)) {
					q_data_size = total_size;
					total_size -= total_size;
				} else {
					q_data_size = size;
					total_size -= size;
				}

				if (q_data_size >= pinfo[port_id].buff_size) {
					if (q_data_size %
						pinfo[port_id].buff_size) {
						mm_tdesc = (q_data_size /
						pinfo[port_id].buff_size) + 1;

						mm_ld_size = q_data_size %
						pinfo[port_id].buff_size;
					} else
						mm_tdesc = (q_data_size /
						pinfo[port_id].buff_size);
				} else {
					mm_tdesc = 1;
					mm_ld_size = q_data_size %
						     pinfo[port_id].buff_size;
				}

				if (port_id)
					offset = (input_size/num_queues) * j;
				else
					offset = (input_size/num_queues) * i;

				ret_val = lseek(ofd, offset, SEEK_SET);
				if (ret_val == (off_t)-1) {
					cmdline_printf(cl, "DMA-to-Device: "
							"lseek func failed\n");
					close(ofd);
					return;
				}
#ifdef PERF_BENCHMARK
				src_addr = (src_addr + (i * q_data_size)) %
							BRAM_SIZE;
#else
				src_addr = src_addr + (i * q_data_size);
#endif
				ret =
				update_queue_param(&rte_eth_devices[port_id], i,
						     RX_SRC_ADDRESS, src_addr);
				if (ret < 0) {
					close(ofd);
					return;
				}

				cmdline_printf(cl, "DMA-from-Device: with "
						"input-size:%d, ld_size:%d, "
						"tot_num_desc:%d\n",
						input_size,  mm_ld_size,
						mm_tdesc);

				if ((unsigned int)i <
					(pinfo[port_id].st_queues))
					ret = do_recv_st(port_id, ofd, i,
								q_data_size);
				else
					ret = do_recv_mm(port_id, ofd, i,
								mm_ld_size,
								mm_tdesc);
				if (ret < 0) {
					close(ofd);
					return;
				}

				ret_val = lseek(ofd, offset, SEEK_END);
				if (ret_val == (off_t)-1) {
					cmdline_printf(cl, "DMA-to-Device: "
							"lseek func failed\n");
					close(ofd);
					return;
				}
			}
			++loop;
		} while (loop < num_loops);
		close(ofd);
	}
	cmdline_printf(cl, "\n####### DMA transfer from device is completed "
						"successfully #######\n");
}

cmdline_parse_token_string_t cmd_obj_action_dma_from_device =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_from_device_result, action,
						"dma_from_device");
cmdline_parse_token_string_t cmd_obj_dma_from_device_port_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_from_device_result, port_id,
								NULL);
cmdline_parse_token_string_t cmd_obj_dma_from_device_queues =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_from_device_result, queues,
								NULL);
cmdline_parse_token_string_t cmd_obj_dma_from_device_filename =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_from_device_result,
							filename, NULL);
cmdline_parse_token_string_t cmd_obj_dma_from_device_src_addr =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_from_device_result,
							src_addr, NULL);
cmdline_parse_token_string_t cmd_obj_dma_from_device_size =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_from_device_result, size,
								NULL);
cmdline_parse_token_string_t cmd_obj_dma_from_device_loops =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_dma_from_device_result, loops,
								NULL);

cmdline_parse_inst_t cmd_obj_dma_from_device = {
	.f = cmd_obj_dma_from_device_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "dma_from_device port_id num-queues filename "
				"src_addr size loops",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_obj_action_dma_from_device,
		(void *)&cmd_obj_dma_from_device_port_id,
		(void *)&cmd_obj_dma_from_device_queues,
		(void *)&cmd_obj_dma_from_device_filename,
		(void *)&cmd_obj_dma_from_device_src_addr,
		(void *)&cmd_obj_dma_from_device_size,
		(void *)&cmd_obj_dma_from_device_loops,
		NULL,
	},

};

/* Command reg_dump addr
 * Register Name - Addr Structure & Values
 */
struct S_NAME_INT_PAIR {
	cmdline_fixed_string_t name;
	int                    offset;
	cmdline_fixed_string_t desc;
};

struct S_NAME_INT_PAIR C_REG_LIST[] = {
	// Build/Status Registers
	{ "FPGA_VER                        ",       0x00000000,
		"FPGA Version" },
	/* Global Ring-size registers */
	{ "QDMA_GLBL_RNG_SZ_A[0]           ",       0x00000204,
		"Ring size index 0" },
	{ "QDMA_GLBL_RNG_SZ_A[1]           ",       0x00000208,
		"Ring size index 1" },
	{ "QDMA_GLBL_RNG_SZ_A[2]           ",       0x0000020c,
		"Ring size index 2" },
	{ "QDMA_GLBL_RNG_SZ_A[3]           ",       0x00000210,
		"Ring size index 3" },
	{ "QDMA_GLBL_RNG_SZ_A[4]           ",       0x00000214,
		"Ring size index 4" },
	{ "QDMA_GLBL_RNG_SZ_A[5]           ",       0x00000218,
		"Ring size index 5" },
	{ "QDMA_GLBL_RNG_SZ_A[6]           ",       0x0000021c,
		"Ring size index 6" },
	{ "QDMA_GLBL_RNG_SZ_A[7]           ",       0x00000220,
		"Ring size index 7" },
	{ "QDMA_GLBL_RNG_SZ_A[8]           ",       0x00000224,
		"Ring size index 8" },
	{ "QDMA_GLBL_RNG_SZ_A[9]           ",       0x00000228,
		"Ring size index 9" },
	{ "QDMA_GLBL_RNG_SZ_A[10]          ",       0x0000022c,
		"Ring size index 10" },
	{ "QDMA_GLBL_RNG_SZ_A[11]          ",       0x00000230,
		"Ring size index 11" },
	{ "QDMA_GLBL_RNG_SZ_A[12]          ",       0x00000234,
		"Ring size index 12" },
	{ "QDMA_GLBL_RNG_SZ_A[13]          ",       0x00000238,
		"Ring size index 13" },
	{ "QDMA_GLBL_RNG_SZ_A[14]          ",       0x0000023c,
		"Ring size index 14" },
	{ "QDMA_GLBL_RNG_SZ_A[15]          ",       0x00000240,
		"Ring size index 15" },
	/* Global Error Status register */
	{ "QDMA_GLBL_ERR_STAT              ",	0x00000248,
		"Global error status" },
	/*Global error mask register*/
	{ "QDMA_GLBL_ERR_MASK              ",	0x0000024c,
		"Global error mask" },
	/* Global Write-back accumulation */
	{ "QDMA_GLBL_WB_ACC                ",	0x00000250,
		"Global Write-back accumulation" },
	/* */
	{ "QDMA_GLBL_DSC_ERR_STS           ",	0x00000254,
		"Global Descriptor error status" },
	/**/
	{ "QDMA_GLBL_DSC_ERR_MSK          ",	0x00000258,
		"Global Descriptor error mask" },
	/**/
	{ "QDMA_GLBL_DSC_ERR_LOG0          ",	0x0000025c,
		"Global Descriptor error Log0" },
	/**/
	{ "QDMA_GLBL_DSC_ERR_LOG1          ",	0x00000260,
		"Global Descriptor error Log1" },
	/**/
	{ "QDMA_GLBL_TRQ_ERR_STS           ",	0x00000264,
		"Global target error status" },
	/**/
	{ "QDMA_GLBL_TRQ_ERR_MSK           ",	0x00000268,
		"Global target error mask"},
	/**/
	{ "QDMA_GLBL_TRQ_ERR_LOG           ",	0x0000026c,
		"Register access space, functio, address" },
	/*Function-map registers*/
	{ "QDMA_TRQ_SEL_FMAP[0]            ",	0x00000400,
		"FMAP target index-0" },
	{ "QDMA_TRQ_SEL_FMAP[1]            ",	0x00000404,
		"FMAP target index-1" },
	{ "QDMA_C2H_STAT_AXIS_C2H_ACPTD    ",	0x00000a88,
		"Number of C2H pkts accepted" },
	{ "QDMA_C2H_STAT_AXIS_CMPT_ACPTD    ",	0x00000a8c,
		"Number of C2H CMPT pkts accepted" },
	{ "QDMA_C2H_STAT_DESC_RSP_PKT_ACPTD",	0x00000a90,
		"Number of desc rsp pkts accepted" },
	{ "QDMA_C2H_STAT_AXIS_PKG_CMP      ",	0x00000a94,
		"Number of C2H pkts accepted" },
	{ "QDMA_C2H_STAT_DESC_RSP_ACPTD    ",	0x00000a98,
		"Number of dsc rsp pkts accepted" },
	{ "QDMA_C2H_STAT_DESC_RSP_CMP      ",	0x00000a9c,
		"Number of dsc rsp pkts completed" },
	{ "QDMA_C2H_STAT_WRQ_OUT           ",	0x00000aa0,
		"Number of WRQ" },
	{ "QDMA_C2H_STAT_WPL_REN_ACPTD     ",	0x00000aa4,
		"Number of WPL REN accepted" },
	{ "QDMA_C2H_STAT_TOT_WRQ_LEN       ",	0x00000aa8,
		"Number of total WRQ len from C2H DMA write engine" },
	{ "QDMA_C2H_STAT_TOT_WPL_LEN       ",	0x00000aac,
		"Number of total WPL len from C2H DMA write engine" },
	{ "QDMA_C2H_ERR_STAT               ",	0x00000af0,
		"C2H error status" },
	{ "QDMA_C2H_ERR_MASK               ",	0x00000af4,
		"C2H error mask" },
	{ "QDMA_C2H_FATAL_ERR_STAT         ",	0x00000af8,
		"C2H fatal error status" },
	{ "QDMA_C2H_FATAL_ERR_MSK          ",	0x00000afc,
		"C2H fatal error mask" },
	{ "QDMA_C2H_FATAL_ERR_MSK          ",	0x00000afc,
		"C2H fatal error mask" },
	// End Of Register List
	{ "EndOfList",     -999,        "EndOfList" }
};


struct cmd_obj_reg_dump_result {
	cmdline_fixed_string_t action;
	cmdline_fixed_string_t port_id;
};

static void cmd_obj_reg_dump_parsed(void *parsed_result,
			       struct cmdline *cl,
			       __attribute__((unused)) void *data)
{
	struct cmd_obj_reg_dump_result *res = parsed_result;


	int bar_id;
	int port_id = atoi(res->port_id);
	unsigned int i, reg_val;
	i     = 0;

	bar_id = pinfo[port_id].config_bar_idx;
	if (bar_id < 0) {
		cmdline_printf(cl, "Error: fetching QDMA config BAR-id "
				"on port-id:%d not supported\n Please enter "
				"valid port-id\n", port_id);
		return;
	}
	cmdline_printf(cl, "Register dump on cofig BAR-id:%d with Port-id:%s\n",
						bar_id, res->port_id);
	if (port_id >= num_ports) {
		cmdline_printf(cl, "Error: port-id:%d not supported\n "
						"Please enter valid port-id\n",
						port_id);
		return;
	}
	cmdline_printf(cl, "FPGA Registers\n--------------\n");
	cmdline_printf(cl, "\tName:\t Offset:\tDescription:\t "
				"register-Value:\n--------------\n");
	do {
		cmdline_printf(cl, "%18s : 0x%08x : %s:",
			       C_REG_LIST[i].name,
			       C_REG_LIST[i].offset,
			       C_REG_LIST[i].desc);
		reg_val = PciRead(bar_id, C_REG_LIST[i].offset, port_id);
		cmdline_printf(cl, "\tValue:0x%08x\n", reg_val);
		i++;
	} while (C_REG_LIST[i].offset != -999);
}

cmdline_parse_token_string_t cmd_obj_action_reg_dump =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_reg_dump_result, action,
								"reg_dump");
cmdline_parse_token_string_t cmd_obj_reg_dump_port_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_reg_dump_result, port_id, NULL);

cmdline_parse_inst_t cmd_obj_reg_dump = {
	.f = cmd_obj_reg_dump_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "reg_dump  port-id",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_obj_action_reg_dump,
		(void *)&cmd_obj_reg_dump_port_id,
		NULL,
	},

};

/*Command queue-context dump*/

#define QDMA_IND_CTXT_CMD_A	0x844

void queue_context_dump(uint8_t bar_id, uint32_t qid, struct cmdline *cl)
{
	union h2c_c2h_ctxt q_ctxt;
	union c2h_cmpt_ctxt c2h_cmpt;
	union h2c_c2h_hw_ctxt hw_ctxt;
	union prefetch_ctxt pref_ctxt;
	uint32_t ctxt_sel, reg_val;
	uint32_t base_offset, offset;
	uint16_t i;
	uint8_t port_id = 0;

	base_offset = (QDMA_TRQ_SEL_IND + QDMA_IND_Q_PRG_OFF);

	/** To read the H2C Queue **/
	ctxt_sel = (QDMA_CTXT_SEL_DESC_SW_H2C<<CTXT_SEL_SHIFT_B) |
				(qid<<QID_SHIFT_B) |
				(QDMA_CTXT_CMD_RD<<OP_CODE_SHIFT_B);
	PciWrite(bar_id, QDMA_IND_CTXT_CMD_A, ctxt_sel, port_id);

	cmdline_printf(cl, "\nH2C context-data structure on queue-id:%d:\n",
									qid);
	for (i = 0; i < 5; i++) {
		offset = base_offset + (i * 4);
		reg_val = PciRead(bar_id, offset, port_id);
		q_ctxt.c_data.data[i] = reg_val;
	}
	cmdline_printf(cl, "\t\t interrupt vector:%x\n",
					q_ctxt.c_fields.int_vec);
	cmdline_printf(cl, "\t\t interrupt aggregation:%x\n",
						q_ctxt.c_fields.int_aggr);
	cmdline_printf(cl, "\t\t Base-addr of Desc ring:%lx\n",
						q_ctxt.c_fields.dsc_base);
	cmdline_printf(cl, "\t\t is_mm:%x\n", q_ctxt.c_fields.is_mm);
	cmdline_printf(cl, "\t\t mrkr_dis:%x\n", q_ctxt.c_fields.mrkr_dis);
	cmdline_printf(cl, "\t\t irq_req:%x\n", q_ctxt.c_fields.irq_req);
	cmdline_printf(cl, "\t\t err-wb-sent:%x\n",
						q_ctxt.c_fields.err_wb_sent);
	cmdline_printf(cl, "\t\t Error status:%x\n", q_ctxt.c_fields.err);
	cmdline_printf(cl, "\t\t irq_no_last:%x\n",
						q_ctxt.c_fields.irq_no_last);
	cmdline_printf(cl, "\t\t port id:%x\n", q_ctxt.c_fields.port_id);
	cmdline_printf(cl, "\t\t irq-enable:%x\n", q_ctxt.c_fields.irq_en);
	cmdline_printf(cl, "\t\t write-back enable:%x\n",
						q_ctxt.c_fields.wbk_en);
	cmdline_printf(cl, "\t\t mm-channel-id:%x\n", q_ctxt.c_fields.mm_chn);
	cmdline_printf(cl, "\t\t bypass:%x\n", q_ctxt.c_fields.byp);
	cmdline_printf(cl, "\t\t desc-size index:%x\n", q_ctxt.c_fields.dsc_sz);
	cmdline_printf(cl, "\t\t ring-size index:%x\n", q_ctxt.c_fields.rng_sz);
	cmdline_printf(cl, "\t\t reserved:%x\n", q_ctxt.c_fields.rsv1);
	cmdline_printf(cl, "\t\t fetch_max:%x\n", q_ctxt.c_fields.fetch_max);
	cmdline_printf(cl, "\t\t address type of context :%x\n",
							q_ctxt.c_fields.at);
	cmdline_printf(cl, "\t\t wbi_acc_en:%x\n", q_ctxt.c_fields.wbi_acc_en);
	cmdline_printf(cl, "\t\t wbi_chk:%x\n", q_ctxt.c_fields.wbi_chk);
	cmdline_printf(cl, "\t\t fetch credits:%x\n", q_ctxt.c_fields.fcrd_en);
	cmdline_printf(cl, "\t\t queue-enable:%x\n", q_ctxt.c_fields.qen);
	cmdline_printf(cl, "\t\t reserved:%x\n", q_ctxt.c_fields.rsv0);
	cmdline_printf(cl, "\t\t function-id:%x\n", q_ctxt.c_fields.fnc_id);
	cmdline_printf(cl, "\t\t irq_ack:%x\n", q_ctxt.c_fields.irq_ack);
	cmdline_printf(cl, "\t\t producer-index:0x%x\n", q_ctxt.c_fields.pidx);

	cmdline_printf(cl, "\nH2C Hardware Descriptor context-data structure "
						"on queue-id:%d:\n", qid);

	ctxt_sel = (QDMA_CTXT_SEL_DESC_HW_H2C<<CTXT_SEL_SHIFT_B) |
				(qid<<QID_SHIFT_B) |
				(QDMA_CTXT_CMD_RD<<OP_CODE_SHIFT_B);
	PciWrite(bar_id, QDMA_IND_CTXT_CMD_A, ctxt_sel, port_id);

	for (i = 0; i < 2; i++) {
		offset = base_offset + (i * 4);
		reg_val = PciRead(bar_id, offset, port_id);
		hw_ctxt.c_data.data[i] = reg_val;
	}
	cmdline_printf(cl, "\t\t reserved:0x%x\n", hw_ctxt.c_fields.rsvd1);
	cmdline_printf(cl, "\t\t descriptor fetch pending:0x%x\n",
						hw_ctxt.c_fields.fetch_pend);
	cmdline_printf(cl, "\t\t event pending:0x%x\n",
						hw_ctxt.c_fields.event_pend);
	cmdline_printf(cl, "\t\t Queue invalid Or no descriptor pending:0x%x\n",
						hw_ctxt.c_fields.idl_stp_b);
	cmdline_printf(cl, "\t\t descriptor pending:0x%x\n",
						hw_ctxt.c_fields.pnd);
	cmdline_printf(cl, "\t\t reserved:0x%x\n", hw_ctxt.c_fields.rsvd0);
	cmdline_printf(cl, "\t\t credit-use:0x%x\n", hw_ctxt.c_fields.crd_use);
	cmdline_printf(cl, "\t\t consumer-index:0x%x\n", hw_ctxt.c_fields.cidx);

	/** To read the C2H Queue **/
	cmdline_printf(cl, "\nC2H context-data structure on queue-id:%d:\n",
									qid);

	ctxt_sel = (QDMA_CTXT_SEL_DESC_SW_C2H<<CTXT_SEL_SHIFT_B) |
					(qid<<QID_SHIFT_B) |
					(QDMA_CTXT_CMD_RD<<OP_CODE_SHIFT_B);
	PciWrite(bar_id, QDMA_IND_CTXT_CMD_A, ctxt_sel, port_id);

	for (i = 0; i < 5; i++) {
		offset = base_offset + (i * 4);
		reg_val = PciRead(bar_id, offset, port_id);
		q_ctxt.c_data.data[i] = reg_val;
	}
	cmdline_printf(cl, "\t\t interrupt vector:%x\n",
						q_ctxt.c_fields.int_vec);
	cmdline_printf(cl, "\t\t interrupt aggregation:%x\n",
						q_ctxt.c_fields.int_aggr);
	cmdline_printf(cl, "\t\t Base-addr of Desc ring:%lx\n",
						q_ctxt.c_fields.dsc_base);
	cmdline_printf(cl, "\t\t is_mm:%x\n", q_ctxt.c_fields.is_mm);
	cmdline_printf(cl, "\t\t mrkr_dis:%x\n", q_ctxt.c_fields.mrkr_dis);
	cmdline_printf(cl, "\t\t irq_req:%x\n", q_ctxt.c_fields.irq_req);
	cmdline_printf(cl, "\t\t err-wb-sent:%x\n",
						q_ctxt.c_fields.err_wb_sent);
	cmdline_printf(cl, "\t\t Error status:%x\n", q_ctxt.c_fields.err);
	cmdline_printf(cl, "\t\t irq_no_last:%x\n",
						q_ctxt.c_fields.irq_no_last);
	cmdline_printf(cl, "\t\t port id:%x\n", q_ctxt.c_fields.port_id);
	cmdline_printf(cl, "\t\t irq-enable:%x\n", q_ctxt.c_fields.irq_en);
	cmdline_printf(cl, "\t\t write-back enable:%x\n",
						q_ctxt.c_fields.wbk_en);
	cmdline_printf(cl, "\t\t mm-channel-id:%x\n", q_ctxt.c_fields.mm_chn);
	cmdline_printf(cl, "\t\t bypass:%x\n", q_ctxt.c_fields.byp);
	cmdline_printf(cl, "\t\t desc-size index:%x\n",
						q_ctxt.c_fields.dsc_sz);
	cmdline_printf(cl, "\t\t ring-size index:%x\n",
						q_ctxt.c_fields.rng_sz);
	cmdline_printf(cl, "\t\t reserved:%x\n", q_ctxt.c_fields.rsv1);
	cmdline_printf(cl, "\t\t fetch_max:%x\n", q_ctxt.c_fields.fetch_max);
	cmdline_printf(cl, "\t\t address type of context :%x\n",
						q_ctxt.c_fields.at);
	cmdline_printf(cl, "\t\t wbi_acc_en:%x\n", q_ctxt.c_fields.wbi_acc_en);
	cmdline_printf(cl, "\t\t wbi_chk:%x\n", q_ctxt.c_fields.wbi_chk);
	cmdline_printf(cl, "\t\t fetch credits:%x\n", q_ctxt.c_fields.fcrd_en);
	cmdline_printf(cl, "\t\t queue-enable:%x\n", q_ctxt.c_fields.qen);
	cmdline_printf(cl, "\t\t reserved:%x\n", q_ctxt.c_fields.rsv0);
	cmdline_printf(cl, "\t\t function-id:%x\n", q_ctxt.c_fields.fnc_id);
	cmdline_printf(cl, "\t\t irq_ack:%x\n", q_ctxt.c_fields.irq_ack);
	cmdline_printf(cl, "\t\t producer-index:0x%x\n", q_ctxt.c_fields.pidx);


	/** C2H Completion context **/
	cmdline_printf(cl, "\nC2H Completion context-data structure "
						"on queue-id:%d:\n", qid);

	ctxt_sel = (QDMA_CTXT_SEL_DESC_CMPT << CTXT_SEL_SHIFT_B) |
		    (qid<<QID_SHIFT_B) | (QDMA_CTXT_CMD_RD<<OP_CODE_SHIFT_B);
	PciWrite(bar_id, QDMA_IND_CTXT_CMD_A, ctxt_sel, port_id);

	for (i = 0; i < 5; i++) {
		offset = base_offset + (i * 4);
		reg_val = PciRead(bar_id, offset, port_id);
		c2h_cmpt.c_data.data[i] = reg_val;
	}

	cmdline_printf(cl, "\t\t at:%x\n", c2h_cmpt.c_fields.at);
	cmdline_printf(cl, "\t\t ovf_chk_dis:%x\n",
			c2h_cmpt.c_fields.ovf_chk_dis);
	cmdline_printf(cl, "\t\t full_upd:%x\n", c2h_cmpt.c_fields.full_upd);
	cmdline_printf(cl, "\t\t timer_run:%x\n", c2h_cmpt.c_fields.timer_run);
	cmdline_printf(cl, "\t\t usr_trig_pend:%x\n",
			c2h_cmpt.c_fields.usr_trig_pend);
	cmdline_printf(cl, "\t\t err:%x\n", c2h_cmpt.c_fields.err);
	cmdline_printf(cl, "\t\t valid:%x\n", c2h_cmpt.c_fields.valid);
	cmdline_printf(cl, "\t\t consumer-index:0x%x\n",
			c2h_cmpt.c_fields.cidx);
	cmdline_printf(cl, "\t\t producer-index:0x%x\n",
			c2h_cmpt.c_fields.pidx);
	cmdline_printf(cl, "\t\t desc-size:%x\n", c2h_cmpt.c_fields.desc_sz);
	cmdline_printf(cl, "\t\t cmpt-desc-base_h addr:0x%x\n",
			(unsigned int)c2h_cmpt.c_fields.cmpt_dsc_base_h);
	cmdline_printf(cl, "\t\t cmpt-desc-base_l addr:0x%x\n",
			(unsigned int)c2h_cmpt.c_fields.cmpt_dsc_base_l);
	cmdline_printf(cl, "\t\t size-index:%x\n", c2h_cmpt.c_fields.size);
	cmdline_printf(cl, "\t\t color:%x\n", c2h_cmpt.c_fields.color);
	cmdline_printf(cl, "\t\t interrupt-state:%x\n",
			c2h_cmpt.c_fields.int_st);
	cmdline_printf(cl, "\t\t timer-index:0x%x\n",
			c2h_cmpt.c_fields.timer_idx);
	cmdline_printf(cl, "\t\t counter-index:0x%x\n",
			c2h_cmpt.c_fields.count_idx);
	cmdline_printf(cl, "\t\t function-id:0x%x\n", c2h_cmpt.c_fields.fnc_id);
	cmdline_printf(cl, "\t\t trigger-mode:0x%x\n",
			c2h_cmpt.c_fields.trig_mode);
	cmdline_printf(cl, "\t\t cause interrupt on completion:0x%x\n",
			c2h_cmpt.c_fields.en_int);
	cmdline_printf(cl, "\t\t cause status descriptor write on "
			"completion:0x%x\n", c2h_cmpt.c_fields.en_stat_desc);

	/* Prefetch Context */
	cmdline_printf(cl, "\nPrefetch context-data structure on "
			"queue-id:%d:\n", qid);

	ctxt_sel = (QDMA_CTXT_SEL_PFTCH << CTXT_SEL_SHIFT_B) |
			(qid << QID_SHIFT_B) |
			(QDMA_CTXT_CMD_RD << OP_CODE_SHIFT_B);
	PciWrite(bar_id, QDMA_IND_CTXT_CMD_A, ctxt_sel, port_id);

	for (i = 0; i < 2; i++) {
		offset = base_offset + (i * 4);
		reg_val = PciRead(bar_id, offset, port_id);
		pref_ctxt.c_data.data[i] = reg_val;
	}
	cmdline_printf(cl, "\t\t valid:0x%x\n", pref_ctxt.c_fields.valid);
	cmdline_printf(cl, "\t\t software credit:0x%x\n",
					pref_ctxt.c_fields.sw_crdt);
	cmdline_printf(cl, "\t\t queue is in prefetch:0x%x\n",
					pref_ctxt.c_fields.pfch);
	cmdline_printf(cl, "\t\t enable prefetch:0x%x\n",
					pref_ctxt.c_fields.pfch_en);
	cmdline_printf(cl, "\t\t err:0x%x\n", pref_ctxt.c_fields.err);
	cmdline_printf(cl, "\t\t rsvd:0x%x\n", pref_ctxt.c_fields.rsvd);
	cmdline_printf(cl, "\t\t port ID:0x%x\n", pref_ctxt.c_fields.port_id);
	cmdline_printf(cl, "\t\t buffer size index:0x%x\n",
					pref_ctxt.c_fields.buf_sz_idx);
	cmdline_printf(cl, "\t\t C2H is in bypass mode:0x%x\n",
					pref_ctxt.c_fields.bypass);

	/* C2H Hardware descriptor context */
	cmdline_printf(cl, "\nC2H Hardware Descriptor context-data structure "
						"on queue-id:%d:\n", qid);

	ctxt_sel = (QDMA_CTXT_SEL_DESC_HW_C2H<<CTXT_SEL_SHIFT_B) |
				(qid<<QID_SHIFT_B) |
				(QDMA_CTXT_CMD_RD<<OP_CODE_SHIFT_B);
	PciWrite(bar_id, QDMA_IND_CTXT_CMD_A, ctxt_sel, port_id);

	for (i = 0; i < 2; i++) {
		offset = base_offset + (i * 4);
		reg_val = PciRead(bar_id, offset, port_id);
		hw_ctxt.c_data.data[i] = reg_val;
	}
	cmdline_printf(cl, "\t\t reserved:0x%x\n", hw_ctxt.c_fields.rsvd1);
	cmdline_printf(cl, "\t\t descriptor fetch pending:0x%x\n",
						hw_ctxt.c_fields.fetch_pend);
	cmdline_printf(cl, "\t\t event pending:0x%x\n",
						hw_ctxt.c_fields.event_pend);
	cmdline_printf(cl, "\t\t Queue invalid Or no descriptor pending:0x%x\n",
						hw_ctxt.c_fields.idl_stp_b);
	cmdline_printf(cl, "\t\t descriptor pending:0x%x\n",
						hw_ctxt.c_fields.pnd);
	cmdline_printf(cl, "\t\t reserved:0x%x\n", hw_ctxt.c_fields.rsvd0);
	cmdline_printf(cl, "\t\t credit-use:0x%x\n", hw_ctxt.c_fields.crd_use);
	cmdline_printf(cl, "\t\t consumer-index:0x%x\n", hw_ctxt.c_fields.cidx);
}

struct cmd_obj_queue_dump_result {
	cmdline_fixed_string_t action;
	cmdline_fixed_string_t port_id;
	cmdline_fixed_string_t queue_id;
};

static void cmd_obj_queue_dump_parsed(void *parsed_result,
			       struct cmdline *cl,
			       __attribute__((unused)) void *data)
{
	struct cmd_obj_queue_dump_result *res = parsed_result;

	cmdline_printf(cl, "queue-dump on Port:%s, queue-id:%s\n\n",
						res->port_id, res->queue_id);

	{
		int port_id = atoi(res->port_id);
		int qid = atoi(res->queue_id);
		int bar_id = 0x0;

		bar_id = pinfo[port_id].config_bar_idx;
		if (bar_id < 0) {
			cmdline_printf(cl, "Error: fetching QDMA config BAR-id "
					"on port-id:%d not supported\n Please "
					"enter valid port-id\n", port_id);
			return;
		}
		if (port_id >= num_ports) {
			cmdline_printf(cl, "Error: port-id:%d not supported\n "
					"Please enter valid port-id\n",
					port_id);
			return;
		}
		if ((unsigned int)qid >= pinfo[port_id].num_queues) {
			cmdline_printf(cl, "Error: queue-id:%d is greater than "
					"the number of confgiured queues in "
					"the port\n Please enter valid "
					"queue-id\n", qid);
			return;
		}
		qid = qid + pinfo[port_id].queue_base;
		queue_context_dump(bar_id, qid, cl);

	}
}

cmdline_parse_token_string_t cmd_obj_action_queue_dump =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_queue_dump_result, action,
								"queue_dump");
cmdline_parse_token_string_t cmd_obj_queue_dump_port_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_queue_dump_result, port_id,
									NULL);
cmdline_parse_token_string_t cmd_obj_queue_dump_queue_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_queue_dump_result, queue_id,
									NULL);

cmdline_parse_inst_t cmd_obj_queue_dump = {
	.f = cmd_obj_queue_dump_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "queue_dump port-id queue_id",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_obj_action_queue_dump,
		(void *)&cmd_obj_queue_dump_port_id,
		(void *)&cmd_obj_queue_dump_queue_id,
		NULL,
	},

};

/* Command descriptor dump */

struct cmd_obj_desc_dump_result {
	cmdline_fixed_string_t action;
	cmdline_fixed_string_t port_id;
	cmdline_fixed_string_t queue_id;
};

static void cmd_obj_desc_dump_parsed(void *parsed_result,
			       struct cmdline *cl,
			       __attribute__((unused)) void *data)
{
	struct cmd_obj_desc_dump_result *res = parsed_result;

	cmdline_printf(cl, "Descriptor-dump on Port:%s, queue-id:%s\n\n",
						res->port_id, res->queue_id);
	{
		int port_id = atoi(res->port_id);
		int qid = atoi(res->queue_id);
		if (port_id >= num_ports) {
			cmdline_printf(cl, "Error: port-id:%d not supported\n"
					"Please enter valid port-id\n",
					port_id);
			return;
		}
		if ((unsigned int)qid >= pinfo[port_id].num_queues) {
			cmdline_printf(cl, "Error: queue-id:%d is greater than "
					"the number of confgiured queues in "
					"the port\n Please enter valid "
					"queue-id\n", qid);
			return;
		}
		qdma_desc_dump(&rte_eth_devices[port_id], qid);
	}
}

cmdline_parse_token_string_t cmd_obj_action_desc_dump =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_desc_dump_result, action,
								"desc_dump");
cmdline_parse_token_string_t cmd_obj_desc_dump_port_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_desc_dump_result, port_id,
									NULL);
cmdline_parse_token_string_t cmd_obj_desc_dump_queue_id =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_desc_dump_result, queue_id,
									NULL);

cmdline_parse_inst_t cmd_obj_desc_dump = {
	.f = cmd_obj_desc_dump_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "desc_dump port-id queue_id",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_obj_action_desc_dump,
		(void *)&cmd_obj_desc_dump_port_id,
		(void *)&cmd_obj_desc_dump_queue_id,
		NULL,
	},

};

/*Command load commands from file */

struct cmd_obj_load_cmds_result {
	cmdline_fixed_string_t action;
	cmdline_fixed_string_t filename;
};

static void cmd_obj_load_cmds_parsed(void *parsed_result,
			       struct cmdline *cl,
			       __attribute__((unused)) void *data)
{
	struct cmd_obj_load_cmds_result *res = parsed_result;
	FILE *fp;
	char buff[256];

	cmdline_printf(cl, "load-cmds from file:%s\n\n", res->filename);
	fp = fopen((const char *)res->filename, "r");
	if (fp == NULL) {
		cmdline_printf(cl, "Error: Invalid filename: %s\n",
							res->filename);
		return;
	}

	rdline_reset(&cl->rdl);
	{
		cmdline_in(cl, "\r", 1);
		while (fgets(buff, sizeof(buff), fp))
			cmdline_in(cl, buff, strlen(buff));

		cmdline_in(cl, "\r", 1);
	}
	fclose(fp);
}

cmdline_parse_token_string_t cmd_obj_action_load_cmds =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_load_cmds_result, action,
								"load_cmds");
cmdline_parse_token_string_t cmd_obj_load_cmds_filename =
	TOKEN_STRING_INITIALIZER(struct cmd_obj_load_cmds_result, filename,
									NULL);

cmdline_parse_inst_t cmd_obj_load_cmds = {
	.f = cmd_obj_load_cmds_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "load_cmds file-name",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_obj_action_load_cmds,
		(void *)&cmd_obj_load_cmds_filename,
		NULL,
	},

};

/* CONTEXT (list of instruction) */

cmdline_parse_ctx_t main_ctx[] = {
	(cmdline_parse_inst_t *)&cmd_obj_port_init,
	(cmdline_parse_inst_t *)&cmd_obj_port_close,
	(cmdline_parse_inst_t *)&cmd_obj_reg_read,
	(cmdline_parse_inst_t *)&cmd_obj_reg_write,
	(cmdline_parse_inst_t *)&cmd_obj_dma_to_device,
	(cmdline_parse_inst_t *)&cmd_obj_dma_from_device,
	(cmdline_parse_inst_t *)&cmd_obj_reg_dump,
	(cmdline_parse_inst_t *)&cmd_obj_queue_dump,
	(cmdline_parse_inst_t *)&cmd_obj_desc_dump,
	(cmdline_parse_inst_t *)&cmd_obj_load_cmds,
	(cmdline_parse_inst_t *)&cmd_help,
	NULL,
};

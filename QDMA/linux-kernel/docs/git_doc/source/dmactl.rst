***********************
DMA Control Application
***********************

QDMA driver comes with a command-line configuration utility called ``dmactl`` to manage the driver.

The Xilinx QDMA control tool, ``dmactl`` is a command Line utility which is installed in ``/usr/local/sbin/`` and allows administration of the Xilinx QDMA queues. Make sure that the installation path ``/usr/local/sbin/`` is added to the ``PATH`` variable.

It can perform the following functions:

- Query the list of QDMA driver binded functions 
- Query control and configuration 

   - List all the queues on a device
   - Add/configure a new queue on a device
   - Start an already added/configured queue, i.e., bring the queue to on line mode
   - Stop a started queue, i.e., bring the queue to off line mode
   - Delete an already configured queue
   
- Register access

   - Read a register
   - Write a register
   - Dump the qdma configuration bar and user bar registers
   
- Debug helper

   - Display a queue's configuration parameters
   - Display a queue's descriptor ring entries
   - Display a c2h queue's completion ring entries
   - Display the interrupt ring entries 

For dmactl help run ``dmactl –h``

For more details on the dmactl tool commands and options for each command, refer to dmactl man page.

For dmactl man page, run ``man dmactl``

==========================
Device Management Commands
==========================

List devices
------------

command: ``dmactl dev list``

This command lists all the physical and virtual functions available in the system along with the number of queues assigned to each function, queue base and queue max.
Each qdma device name is listed as ``qdma<bbddf>`` where ``<bbddf>`` is bus number, device number and function number.

Initially when the system is started, the queues are not assigned to any of the available functions.

::


	[xilinx@]# dmactl dev list

	qdma01000	0000:01:00.0	max QP: 0, -~-
	qdma01001	0000:01:00.1	max QP: 0, -~-
	qdma01002	0000:01:00.2	max QP: 0, -~-
	qdma01003	0000:01:00.3	max QP: 0, -~-


	qdmavf01004	0000:01:00.4	max QP: 0, -~-
	

	[xilinx@]# echo 100 > /sys/bus/pci/devices/0000\:01\:00.0/qdma/qmax 
	[xilinx@]# echo 100 > /sys/bus/pci/devices/0000\:01\:00.1/qdma/qmax 
	[xilinx@]# echo 100 > /sys/bus/pci/devices/0000\:01\:00.2/qdma/qmax 
	[xilinx@]# echo 100 > /sys/bus/pci/devices/0000\:01\:00.3/qdma/qmax 
	[xilinx@]# echo 100 > /sys/bus/pci/devices/0000\:01\:00.4/qdma/qmax 
	[xilinx@]# dmactl dev list

	qdma01000	0000:01:00.0	max QP: 100, 0~99
	qdma01001	0000:01:00.1	max QP: 100, 100~199
	qdma01002	0000:01:00.2	max QP: 100, 200~299
	qdma01003	0000:01:00.3	max QP: 100, 300~399


	qdmavf01004	0000:01:00.4	max QP: 100, 400~499



=========================
Queue Management Commands
=========================

List Version
------------

command: ``dmactl qdma01000 version``

This command lists hardware and software version details and device capabilities.
::

	[xilinx@]# dmactl qdma01000 devinfo
	=============Hardware Version============

	RTL Version         : RTL Base
	Vivado ReleaseID    : vivado 2019.1
	Everest IP          : Soft IP

	============Software Version============

	qdma driver version : 2019.1.121.205.

	=============Hardware Capabilities============

	Number of PFs supported                : 4
	Total number of queues supported       : 2048
	MM channels                            : 1
	FLR Present                            : yes
	ST enabled                             : yes
	MM enabled                             : yes
	Mailbox enabled                        : yes
	MM completion enabled                  : no



List Device Statistics
-----------------------

command: ``dmactl qdma01000 stat``

This command lists the statistics accumulated for this device
::

   [xilinx@]# dmactl qdma01000 stat

   qdma01000:statistics
   Total MM H2C packets processed = 312220
   Total MM C2H packets processed = 312220
   Total ST H2C packets processed = 64127
   Total ST C2H packets processed = 100954

Use ``dmactl qdma01000 stat clear`` to clear the statistics collected

Add a Queue
-----------

command: ``dmactl qdma01000 q add idx <N> [mode <st|mm>] [dir <h2c|c2h|bi>]``

This command allows the user to add a queue.

**Parameters**

- <N> : Queue number
- mode : mode of the queue, streaming\(st\) or memory mapped\(mm\). Mode defaults to mm.
- dir : Direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\). Direction defaults to h2c.

::

   [xilinx@]# dmactl qdma01000 q add idx 4 mode mm dir h2c

   qdma01000-MM-4 H2C added.
   Added 1 Queues.

Add a List of Queues
--------------------

command: ``dmactl qdma01000 q add list <start_idx> <N>  [ mode <st|mm> ] [ dir <h2c|c2h|bi> ]``

This command allows the user to add a list of queues.

**Parameters**

- <start_idx> : Starting queue number
- <N> :Number of queues to add
- mode : mode of the queue, streaming\(st\) or memory mapped\(mm\)
- dir : Direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\)

::

   [xilinx@]# dmactl qdma01000 q add list 1 4 mode mm dir h2c

   qdma01000-MM-1 H2C added.
   qdma01000-MM-2 H2C added.
   qdma01000-MM-3 H2C added.
   qdma01000-MM-4 H2C added.
   Added 4 Queues.
   
Start a Queue
-------------

command: 
   dmactl qdma01000 q start idx <N> [dir <h2c|c2h|bi>] [en_mm_cmpl] [idx_ringsz <0:15>] [idx_bufsz <0:15>] [idx_tmr <0:15>] \
        [idx_cntr <0:15>] [trigmode <every|usr_cnt|usr|usr_tmr|dis>] [cmptsz <0|1|2|3>] [sw_desc_sz <3>] \
        [desc_bypass_en] [pfetch_en] [pfetch_bypass_en] [dis_cmpl_status] \
        [dis_cmpl_status_acc] [dis_cmpl_status_pend_chk] [c2h_udd_en] \
        [cmpl_ovf_dis] [dis_fetch_credit] [dis_cmpt_stat] [c2h_cmpl_intr_en]

This command allows the user to start a queue.

**Parameters**

- <N> : Queue number
- dir : Direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\). Directions defaults to h2c.
- en_mm_cmpl : Enable MM completions. This is valid only for MM Completion example design.
- idx_ringsz: CSR register ring size index
- idx_bufsz : CSR register buffer size index
- idx_tmr : CSR register timer index
- idx_cntr: CSR register counter index
- trigmode: Timer trigger mode \(every, user counter, user, user timer, disabled\)
- cmptsz : Completion size \( 0: 8 bytes, 1: 16 bytes, 2:32 bytes, 3:64 bytes\)
- sw_desc_sz : Descriptor size
- desc_bypass_en : Enable descriptor bypass
- pfetch_en : Enable prefetch
- pfetch_bypass_en : Enable prefetch bypass
- dis_cmpl_status : Disable completion status update
- dis_cmpl_status_acc : Disable completion status accumulation
- dis_cmpl_status_pend_chk : Disable completion status pending check
- c2h_udd_en : Enable immediate data\(User Defined Data\)
- dis_fetch_credit: Disable fetch credit
- dis_cmpt_stat : Disable completion status
- c2h_cmpl_intr_en : Enable c2h completion interval
- cmpl_ovf_dis : Disable completion over flow check

::

   [xilinx@]# dmactl qdma01000 q start idx 4 dir h2c
   dmactl: Info: Default ring size set to 2048

   1 Queues started, idx 4 ~ 4.

Start a List of Queues
----------------------

command:
   dmactl qdma01000 q start list <start_idx> <N> [dir <h2c|c2h|bi>]  [en_mm_cmpl] [idx_ringsz <0:15>] [idx_bufsz <0:15>] [idx_tmr <0:15>] \
   [idx_cntr <0:15>] [trigmode <every|usr_cnt|usr|usr_tmr|dis>] [cmptsz <0|1|2|3>] [desc_bypass_en] [pfetch_en] [pfetch_bypass_en]\
   [dis_cmpl_status] [dis_cmpl_status_acc] [dis_cmpl_status_pend_chk] [c2h_udd_en] [dis_fetch_credit] [dis_cmpt_stat] [c2h_cmpl_intr_en] \ [cmpl_ovf_dis]

This command allows the user to start a list of queues.

**Parameters**

- <start_idx> : Starting queue number
- <N> :Number of queues to delete
- dir : direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\). Directions defaults to h2c.
- en_mm_cmpl : Enable MM completions. This is valid only for MM Completion example design.
- idx_ringsz: CSR register ring size index
- idx_bufsz : CSR register buffer size index
- idx_tmr : CSR register timer index
- idx_cntr: CSR register counter index
- trigmode: Timer trigger mode \(every, user counter, user, user timer, disabled\)
- cmptsz : Completion size \( 0: 8 bytes, 1: 16 bytes, 2:32 bytes, 3:64 bytes\)
- desc_bypass_en : Enable descriptor bypass
- pfetch_en : Enable prefetch
- pfetch_bypass_en : Enable prefetch bypass
- dis_cmpl_status : Disable completion status update
- dis_cmpl_status_pend_chk : Disable completion status pending check
- c2h_udd_en : Enable immdeiate data\(User Defined Data\)
- dis_fetch_credit: Disable fetch credit
- dis_cmpt_stat : Disable completion status
- c2h_cmpl_intr_en : Enable c2h completion interval
- cmpl_ovf_dis : Disable completion over flow check

::

   [xilinx@]# dmactl qdma01000 q start list 1 4 dir h2c

   Started Queues 1 -> 4.
   
Stop a Queue
------------

command: ``dmactl qdma01000 q stop idx <N> [dir <h2c|c2h|bi>]``

This command allows the user to stop a queue.

**Parameters**

- <N> : Queue number
- dir : direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\). Directions defaults to h2c.

::

   [xilinx@]# dmactl qdma01000 q stop idx 4 dir h2c
   dmactl: Info: Default ring size set to 2048

   Stopped Queues 4 -> 4.
   
Stop a List of Queues
---------------------

command: ``dmactl qdma01000 q stop list <start_idx> <N> [dir <h2c|c2h|bi>]``

This command allows the user to stop a list of queues.

**Parameters**

- <start_idx> : Starting queue number
- <N> : Number of queues to delete
- dir : Direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\). Directions defaults to h2c.

::

   [xilinx@]# dmactl qdma01000 q stop list 1 4 dir h2c

   Stopped Queues 1 -> 4.

Delete a Queue
--------------

command: ``dmactl qdma01000 q del idx <N> [dir <h2c|c2h|bi>]``

This command allows the user to delete a queue.

**Parameters**

- <N> : Queue number
- dir : direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\). Directions defaults to h2c.

::

   [xilinx@]# dmactl qdma01000 q del idx 4 mode mm dir h2c

   Deleted Queues 4 -> 4.
   
Delete a List of Queues
-----------------------

command: ``dmactl qdma01000 q del list <start_idx> <N> [ dir <h2c|c2h|bi> ]``

This command allows the user to delete a list of queues.

**Parameters**

- <start_idx> : Starting queue number
- <N> : Number of queues to delete
- dir : Direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\)

::

   [xilinx@]# dmactl qdma01000 q del list 1 4 dir h2c

   Deleted Queues 1 -> 4.
   
Dump Queue Information
----------------------

command: ``dmactl qdma01000 q dump idx <N> [dir <h2c|c2h|bi>]``

Dump the queue information

**Parameters**

- <N> : Queue number
- dir : Direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\)

Sample output is given below:

::


	[xilinx@]# dmactl qdma01000 q dump idx 1 dir bi

	qdma01000-ST-1 C2H online
		hw_ID 1, thp ?, desc 0xffff880084140000/0x84140000, 1536
		cmpt desc 0xffff8800842a0000/0x842a0000, 2048

		cmpl status: 0xffff880084143000, 00000000 00000000
		CMPT CMPL STATUS: 0xffff8800842a4000, 00000000 00000000
		SOFTWARE CTXT:
			Interrupt Aggregation                           0x1        1
			Ring Index                                      0x0        0
			Descriptor Ring Base Addr (High)                0x0        0
			Descriptor Ring Base Addr (Low)                 0x84140000 2215903232
			Is Memory Mapped                                0x0        0
			Marker Disable                                  0x0        0
			IRQ Request                                     0x0        0
			Writeback Error Sent                            0x0        0
			Error                                           0x0        0
			Interrupt No Last                               0x0        0
			Port Id                                         0x0        0
			Interrupt Enable                                0x0        0
			Writeback Enable                                0x0        0
			MM Channel                                      0x0        0
			Bypass Enable                                   0x0        0
			Descriptor Size                                 0x0        0
			Ring Size                                       0x9        9
			Fetch Max                                       0x0        0
			Address Translation                             0x0        0
			Write back/Intr Interval                        0x1        1
			Write back/Intr Check                           0x0        0
			Fetch Credit Enable                             0x1        1
			Queue Enable                                    0x1        1
			Function Id                                     0x0        0
			IRQ Arm                                         0x0        0
			PIDX                                            0x5ff      1535

		HARDWARE CTXT:
			Fetch Pending                                   0x0        0
			Eviction Pending                                0x0        0
			Queue Invalid No Desc Pending                   0x1        1
			Descriptors Pending                             0x0        0
			Credits Consumed                                0x0        0
			CIDX                                            0x0        0

		CREDIT CTXT:
			Credit                                          0x0        0

		CMPT CTXT:
			Interrupt Aggregation                           0x1        1
			Ring Index                                      0x0        0
			Address Translation                             0x0        0
			Over Flow Check Disable                         0x0        0
			Full Update                                     0x0        0
			Timer Running                                   0x0        0
			Trigger Pending                                 0x0        0
			Error                                           0x0        0
			Valid                                           0x1        1
			CIDX                                            0x0        0
			PIDX                                            0x0        0
			Descriptor Size                                 0x0        0
			Base Address (High)                             0x0        0
			Base Address (Low)                              0x842a0000 2217345024
			Ring Size                                       0x0        0
			Color                                           0x1        1
			Interrupt State                                 0x1        1
			Timer Index                                     0x0        0
			Counter Index                                   0x0        0
			Function Id                                     0x0        0
			Trigger Mode                                    0x1        1
			Enable Interrupt                                0x1        1
			Enable Status Desc Update                       0x1        1

		PREFETCH CTXT:
			Valid                                           0x1        1
			Software Credit                                 0x5ff      1535
			In Prefetch                                     0x0        0
			Prefetch Enable                                 0x0        0
			Error                                           0x0        0
			Port Id                                         0x0        0
			Buffer Size Index                               0x0        0
			Bypass                                          0x0        0

		INTR CTXT:
			at                                              0x0        0
			pidx                                            0x0        0
			page_size                                       0x0        0
			baddr_4k (High)                                 0x0        0
			baddr_4k (Low)                                  0x8414c000 2215952384
			color                                           0x1        1
			int_st                                          0x0        0
			vec                                             0x3        3
			valid                                           0x1        1

		total descriptor processed:    0
	qdma01000-ST-1 H2C online
		hw_ID 1, thp ?, desc 0xffff880084288000/0x84288000, 1536

		cmpl status: 0xffff88008428e000, 00000000 00000000
		SOFTWARE CTXT:
			Interrupt Aggregation                           0x1        1
			Ring Index                                      0x0        0
			Descriptor Ring Base Addr (High)                0x0        0
			Descriptor Ring Base Addr (Low)                 0x84288000 2217246720
			Is Memory Mapped                                0x0        0
			Marker Disable                                  0x0        0
			IRQ Request                                     0x0        0
			Writeback Error Sent                            0x0        0
			Error                                           0x0        0
			Interrupt No Last                               0x0        0
			Port Id                                         0x0        0
			Interrupt Enable                                0x1        1
			Writeback Enable                                0x1        1
			MM Channel                                      0x0        0
			Bypass Enable                                   0x0        0
			Descriptor Size                                 0x1        1
			Ring Size                                       0x9        9
			Fetch Max                                       0x0        0
			Address Translation                             0x0        0
			Write back/Intr Interval                        0x1        1
			Write back/Intr Check                           0x1        1
			Fetch Credit Enable                             0x0        0
			Queue Enable                                    0x1        1
			Function Id                                     0x0        0
			IRQ Arm                                         0x1        1
			PIDX                                            0x0        0

		HARDWARE CTXT:
			Fetch Pending                                   0x0        0
			Eviction Pending                                0x0        0
			Queue Invalid No Desc Pending                   0x1        1
			Descriptors Pending                             0x0        0
			Credits Consumed                                0x0        0
			CIDX                                            0x0        0

		CREDIT CTXT:
			Credit                                          0x0        0

		INTR CTXT:
			at                                              0x0        0
			pidx                                            0x0        0
			page_size                                       0x0        0
			baddr_4k (High)                                 0x0        0
			baddr_4k (Low)                                  0x8414c000 2215952384
			color                                           0x1        1
			int_st                                          0x0        0
			vec                                             0x3        3
			valid                                           0x1        1

		total descriptor processed:    0
	Dumped Queues 1 -> 1.


   
Dump Multiple Queue Information
-------------------------------

command: ``dmactl qdma01000 q dump idx <N> [dir <h2c|c2h|bi>]``

Dumps the information for multiple queues

**Parameters**

- <start_idx> : Starting queue number
- <N> :Number of queues to add
- dir : Direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\)

::



	[xilinx@]# dmactl qdma01000 q dump list 1 2 dir bi

	qdma01000-ST-1 C2H online
		hw_ID 1, thp ?, desc 0xffff880084140000/0x84140000, 1536
		cmpt desc 0xffff8800842a0000/0x842a0000, 2048

		cmpl status: 0xffff880084143000, 00000000 00000000
		CMPT CMPL STATUS: 0xffff8800842a4000, 00000000 00000000
		SOFTWARE CTXT:
			Interrupt Aggregation                           0x1        1
			Ring Index                                      0x0        0
			Descriptor Ring Base Addr (High)                0x0        0
			Descriptor Ring Base Addr (Low)                 0x84140000 2215903232
			Is Memory Mapped                                0x0        0
			Marker Disable                                  0x0        0
			IRQ Request                                     0x0        0
			Writeback Error Sent                            0x0        0
			Error                                           0x0        0
			Interrupt No Last                               0x0        0
			Port Id                                         0x0        0
			Interrupt Enable                                0x0        0
			Writeback Enable                                0x0        0
			MM Channel                                      0x0        0
			Bypass Enable                                   0x0        0
			Descriptor Size                                 0x0        0
			Ring Size                                       0x9        9
			Fetch Max                                       0x0        0
			Address Translation                             0x0        0
			Write back/Intr Interval                        0x1        1
			Write back/Intr Check                           0x0        0
			Fetch Credit Enable                             0x1        1
			Queue Enable                                    0x1        1
			Function Id                                     0x0        0
			IRQ Arm                                         0x0        0
			PIDX                                            0x5ff      1535

		HARDWARE CTXT:
			Fetch Pending                                   0x0        0
			Eviction Pending                                0x0        0
			Queue Invalid No Desc Pending                   0x1        1
			Descriptors Pending                             0x0        0
			Credits Consumed                                0x0        0
			CIDX                                            0x0        0

		CREDIT CTXT:
			Credit                                          0x0        0

		CMPT CTXT:
			Interrupt Aggregation                           0x1        1
			Ring Index                                      0x0        0
			Address Translation                             0x0        0
			Over Flow Check Disable                         0x0        0
			Full Update                                     0x0        0
			Timer Running                                   0x0        0
			Trigger Pending                                 0x0        0
			Error                                           0x0        0
			Valid                                           0x1        1
			CIDX                                            0x0        0
			PIDX                                            0x0        0
			Descriptor Size                                 0x0        0
			Base Address (High)                             0x0        0
			Base Address (Low)                              0x842a0000 2217345024
			Ring Size                                       0x0        0
			Color                                           0x1        1
			Interrupt State                                 0x1        1
			Timer Index                                     0x0        0
			Counter Index                                   0x0        0
			Function Id                                     0x0        0
			Trigger Mode                                    0x1        1
			Enable Interrupt                                0x1        1
			Enable Status Desc Update                       0x1        1

		PREFETCH CTXT:
			Valid                                           0x1        1
			Software Credit                                 0x5ff      1535
			In Prefetch                                     0x0        0
			Prefetch Enable                                 0x0        0
			Error                                           0x0        0
			Port Id                                         0x0        0
			Buffer Size Index                               0x0        0
			Bypass                                          0x0        0

		INTR CTXT:
			at                                              0x0        0
			pidx                                            0x0        0
			page_size                                       0x0        0
			baddr_4k (High)                                 0x0        0
			baddr_4k (Low)                                  0x8414c000 2215952384
			color                                           0x1        1
			int_st                                          0x0        0
			vec                                             0x3        3
			valid                                           0x1        1

		total descriptor processed:    0
	qdma01000-ST-1 H2C online
		hw_ID 1, thp ?, desc 0xffff880084288000/0x84288000, 1536

		cmpl status: 0xffff88008428e000, 00000000 00000000
		SOFTWARE CTXT:
			Interrupt Aggregation                           0x1        1
			Ring Index                                      0x0        0
			Descriptor Ring Base Addr (High)                0x0        0
			Descriptor Ring Base Addr (Low)                 0x84288000 2217246720
			Is Memory Mapped                                0x0        0
			Marker Disable                                  0x0        0
			IRQ Request                                     0x0        0
			Writeback Error Sent                            0x0        0
			Error                                           0x0        0
			Interrupt No Last                               0x0        0
			Port Id                                         0x0        0
			Interrupt Enable                                0x1        1
			Writeback Enable                                0x1        1
			MM Channel                                      0x0        0
			Bypass Enable                                   0x0        0
			Descriptor Size                                 0x1        1
			Ring Size                                       0x9        9
			Fetch Max                                       0x0        0
			Address Translation                             0x0        0
			Write back/Intr Interval                        0x1        1
			Write back/Intr Check                           0x1        1
			Fetch Credit Enable                             0x0        0
			Queue Enable                                    0x1        1
			Function Id                                     0x0        0
			IRQ Arm                                         0x1        1
			PIDX                                            0x0        0

		HARDWARE CTXT:
			Fetch Pending                                   0x0        0
			Eviction Pending                                0x0        0
			Queue Invalid No Desc Pending                   0x1        1
			Descriptors Pending                             0x0        0
			Credits Consumed                                0x0        0
			CIDX                                            0x0        0

		CREDIT CTXT:
			Credit                                          0x0        0

		INTR CTXT:
			at                                              0x0        0
			pidx                                            0x0        0
			page_size                                       0x0        0
			baddr_4k (High)                                 0x0        0
			baddr_4k (Low)                                  0x8414c000 2215952384
			color                                           0x1        1
			int_st                                          0x0        0
			vec                                             0x3        3
			valid                                           0x1        1

		total descriptor processed:    0
	qdma01000-ST-2 C2H online
		hw_ID 2, thp ?, desc 0xffff880084274000/0x84274000, 1536
		cmpt desc 0xffff880084398000/0x84398000, 2048

		cmpl status: 0xffff880084277000, 00000000 00000000
		CMPT CMPL STATUS: 0xffff88008439c000, 00000000 00000000
		SOFTWARE CTXT:
			Interrupt Aggregation                           0x1        1
			Ring Index                                      0x0        0
			Descriptor Ring Base Addr (High)                0x0        0
			Descriptor Ring Base Addr (Low)                 0x84274000 2217164800
			Is Memory Mapped                                0x0        0
			Marker Disable                                  0x0        0
			IRQ Request                                     0x0        0
			Writeback Error Sent                            0x0        0
			Error                                           0x0        0
			Interrupt No Last                               0x0        0
			Port Id                                         0x0        0
			Interrupt Enable                                0x0        0
			Writeback Enable                                0x0        0
			MM Channel                                      0x0        0
			Bypass Enable                                   0x0        0
			Descriptor Size                                 0x0        0
			Ring Size                                       0x9        9
			Fetch Max                                       0x0        0
			Address Translation                             0x0        0
			Write back/Intr Interval                        0x1        1
			Write back/Intr Check                           0x0        0
			Fetch Credit Enable                             0x1        1
			Queue Enable                                    0x1        1
			Function Id                                     0x0        0
			IRQ Arm                                         0x0        0
			PIDX                                            0x5ff      1535

		HARDWARE CTXT:
			Fetch Pending                                   0x0        0
			Eviction Pending                                0x0        0
			Queue Invalid No Desc Pending                   0x1        1
			Descriptors Pending                             0x0        0
			Credits Consumed                                0x0        0
			CIDX                                            0x0        0

		CREDIT CTXT:
			Credit                                          0x0        0

		CMPT CTXT:
			Interrupt Aggregation                           0x1        1
			Ring Index                                      0x0        0
			Address Translation                             0x0        0
			Over Flow Check Disable                         0x0        0
			Full Update                                     0x0        0
			Timer Running                                   0x0        0
			Trigger Pending                                 0x0        0
			Error                                           0x0        0
			Valid                                           0x1        1
			CIDX                                            0x0        0
			PIDX                                            0x0        0
			Descriptor Size                                 0x0        0
			Base Address (High)                             0x0        0
			Base Address (Low)                              0x84398000 2218360832
			Ring Size                                       0x0        0
			Color                                           0x1        1
			Interrupt State                                 0x1        1
			Timer Index                                     0x0        0
			Counter Index                                   0x0        0
			Function Id                                     0x0        0
			Trigger Mode                                    0x1        1
			Enable Interrupt                                0x1        1
			Enable Status Desc Update                       0x1        1

		PREFETCH CTXT:
			Valid                                           0x1        1
			Software Credit                                 0x5ff      1535
			In Prefetch                                     0x0        0
			Prefetch Enable                                 0x0        0
			Error                                           0x0        0
			Port Id                                         0x0        0
			Buffer Size Index                               0x0        0
			Bypass                                          0x0        0

		INTR CTXT:
			at                                              0x0        0
			pidx                                            0x0        0
			page_size                                       0x0        0
			baddr_4k (High)                                 0x0        0
			baddr_4k (Low)                                  0x8414c000 2215952384
			color                                           0x1        1
			int_st                                          0x0        0
			vec                                             0x3        3
			valid                                           0x1        1

		total descriptor processed:    0
	qdma01000-ST-2 H2C online
		hw_ID 2, thp ?, desc 0xffff8800843a0000/0x843a0000, 1536

		cmpl status: 0xffff8800843a6000, 00000000 00000000
		SOFTWARE CTXT:
			Interrupt Aggregation                           0x1        1
			Ring Index                                      0x0        0
			Descriptor Ring Base Addr (High)                0x0        0
			Descriptor Ring Base Addr (Low)                 0x843a0000 2218393600
			Is Memory Mapped                                0x0        0
			Marker Disable                                  0x0        0
			IRQ Request                                     0x0        0
			Writeback Error Sent                            0x0        0
			Error                                           0x0        0
			Interrupt No Last                               0x0        0
			Port Id                                         0x0        0
			Interrupt Enable                                0x1        1
			Writeback Enable                                0x1        1
			MM Channel                                      0x0        0
			Bypass Enable                                   0x0        0
			Descriptor Size                                 0x1        1
			Ring Size                                       0x9        9
			Fetch Max                                       0x0        0
			Address Translation                             0x0        0
			Write back/Intr Interval                        0x1        1
			Write back/Intr Check                           0x1        1
			Fetch Credit Enable                             0x0        0
			Queue Enable                                    0x1        1
			Function Id                                     0x0        0
			IRQ Arm                                         0x1        1
			PIDX                                            0x0        0

		HARDWARE CTXT:
			Fetch Pending                                   0x0        0
			Eviction Pending                                0x0        0
			Queue Invalid No Desc Pending                   0x1        1
			Descriptors Pending                             0x0        0
			Credits Consumed                                0x0        0
			CIDX                                            0x0        0

		CREDIT CTXT:
			Credit                                          0x0        0

		INTR CTXT:
			at                                              0x0        0
			pidx                                            0x0        0
			page_size                                       0x0        0
			baddr_4k (High)                                 0x0        0
			baddr_4k (Low)                                  0x8414c000 2215952384
			color                                           0x1        1
			int_st                                          0x0        0
			vec                                             0x3        3
			valid                                           0x1        1

		total descriptor processed:    0
	Dumped Queues 1 -> 2.


   

   
Dump Queue Descriptor Information
---------------------------------

command: ``dmactl qdma01000 q dump idx <N> [dir <h2c|c2h|bi>] [desc <x> <y>]``

Dump the queue descriptor information

**Parameters**

- <N> : Queue number
- dir : Direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\)
- <x> : Range start
- <y> : Range end

::

	[xilinx@]# dmactl qdma17000 q dump idx 1 dir h2c desc 1 10

	qdma17000-MM-1 H2C online
	1: 0x0000000075e985a1 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	2: 0x000000009fa51b7d 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	3: 0x0000000088024b26 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	4: 0x0000000003e7e32a 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	5: 0x0000000017908b59 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	6: 0x000000006010e5f5 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	7: 0x00000000ea16b7aa 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	8: 0x00000000f49eab9e 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	9: 0x000000005867272e 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	CMPL STATUS: 0x000000005a1efda1 00000000 00000000
	Dumped descs of queues 1 -> 1.


   
Dump Multiple Queue Descriptor Information
------------------------------------------

command: ``dmactl qdma01000 q dump list idx <N> [dir <h2c|c2h|bi>] [desc <x> <y>]``

Dumps the descriptor information for multiple queues

**Parameters**

- <start_idx> : Starting queue number
- <N> :Number of queues to add
- dir : Direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\)
- <x> : Range start
- <y> : Range end

::

	[xilinx@]# dmactl qdma17000 q dump list 1 2 dir h2c desc 1 10

	qdma17000-MM-1 H2C online
	1: 0x0000000075e985a1 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	2: 0x000000009fa51b7d 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	3: 0x0000000088024b26 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	4: 0x0000000003e7e32a 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	5: 0x0000000017908b59 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	6: 0x000000006010e5f5 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	7: 0x00000000ea16b7aa 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	8: 0x00000000f49eab9e 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	9: 0x000000005867272e 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	CMPL STATUS: 0x000000005a1efda1 00000000 00000000
	qdma17000-MM-2 H2C online
	1: 0x0000000088caff6d 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	2: 0x0000000023211cbf 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	3: 0x000000003468cd41 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	4: 0x00000000ad729161 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	5: 0x00000000ee3b9e4b 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	6: 0x000000009d302231 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	7: 0x0000000013d70540 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	8: 0x000000004d2f1fe2 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	9: 0x00000000d59589f0 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	CMPL STATUS: 0x00000000026d0732 00000000 00000000
	Dumped descs of queues 1 -> 2.

Dump Queue Completion Information
---------------------------------

command: ``dmactl qdma01000 q dump idx <N> [dir <h2c|c2h|bi>] [cmpt <x> <y>]``

Dump the queue completion information. This command is valid only for streaming c2h.

**Parameters**

- <N> : Queue number
- dir : Direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\)
- <x> : Range start
- <y> : Range end

::

	[xilinx@]# dmactl qdma17000 q dump idx 5 dir c2h cmpt 0 10

	qdma17000-ST-5 C2H online
	0: 0x000000006d62f1aa 00000000 00000000
	1: 0x000000007a07b4ba 00000000 00000000
	2: 0x000000000f158857 00000000 00000000
	3: 0x00000000489003ed 00000000 00000000
	4: 0x0000000054d4b084 00000000 00000000
	5: 0x000000001e3d17d8 00000000 00000000
	6: 0x000000001e09b4d9 00000000 00000000
	7: 0x000000002cb94242 00000000 00000000
	8: 0x00000000dd831ff4 00000000 00000000
	9: 0x000000006a4748c3 00000000 00000000
	CMPL STATUS: 0x00000000074d569c 00000000 00000000
	Dumped descs of queues 5 -> 5.

Dump Multiple Queue Completion Information
------------------------------------------

command: ``dmactl qdma01000 q dump list idx <N> [dir <h2c|c2h|bi>] [cmpt <x> <y>]``

Dumps the completion information for multiple queues. This command is valid only for streaming c2h.

**Parameters**

- <start_idx> : Starting queue number
- <N> :Number of queues to add
- dir : Direction of the queue, host-to-card\(h2c\), card-to-host \(c2h\) or both \(bi\)
- <x> : Range start
- <y> : Range end

::

	[xilinx@]# dmactl qdma17000 q dump list 5 2 dir c2h cmpt 0 10

	qdma17000-ST-5 C2H online
	0: 0x000000006d62f1aa 00000000 00000000
	1: 0x000000007a07b4ba 00000000 00000000
	2: 0x000000000f158857 00000000 00000000
	3: 0x00000000489003ed 00000000 00000000
	4: 0x0000000054d4b084 00000000 00000000
	5: 0x000000001e3d17d8 00000000 00000000
	6: 0x000000001e09b4d9 00000000 00000000
	7: 0x000000002cb94242 00000000 00000000
	8: 0x00000000dd831ff4 00000000 00000000
	9: 0x000000006a4748c3 00000000 00000000
	CMPL STATUS: 0x00000000074d569c 00000000 00000000
	qdma17000-ST-6 C2H online
	0: 0x000000004ca5cbb0 00000000 00000000
	1: 0x000000003b6478d7 00000000 00000000
	2: 0x000000007dc4c8a1 00000000 00000000
	3: 0x000000003ad66591 00000000 00000000
	4: 0x00000000aad20103 00000000 00000000
	5: 0x00000000f102be8c 00000000 00000000
	6: 0x0000000046cc60b8 00000000 00000000
	7: 0x000000003dd14944 00000000 00000000
	8: 0x000000004c825f31 00000000 00000000
	9: 0x0000000026f2e4f8 00000000 00000000
	CMPL STATUS: 0x000000007bcad59e 00000000 00000000
	Dumped descs of queues 5 -> 6.

Dump the Interrupt Ring Information
-----------------------------------

command: ``dmactl qdma01000 intring dump vector <N> <start_idx> <end_idx>``

Dump the interrupt ring information

**Parameters**

- <N> : Vector number
- <start_idx> : Range start
- <end_idx> : Range end

::

	[xilinx@]# dmactl qdma17000 intring dump vector 3 0 10

	intr_ring_entry = 0: 0x00000000 0x00000000
	intr_ring_entry = 1: 0x00000000 0x00000000
	intr_ring_entry = 2: 0x00000000 0x00000000
	intr_ring_entry = 3: 0x00000000 0x00000000
	intr_ring_entry = 4: 0x00000000 0x00000000
	intr_ring_entry = 5: 0x00000000 0x00000000
	intr_ring_entry = 6: 0x00000000 0x00000000
	intr_ring_entry = 7: 0x00000000 0x00000000
	intr_ring_entry = 8: 0x00000000 0x00000000
	intr_ring_entry = 9: 0x00000000 0x00000000
	intr_ring_entry = 10: 0x00000000 0x00000000


=================
Register Commands
=================

Read a Register
---------------

command: ``dmactl qdma01000 reg read bar <N> <addr>``

Read a register value.

**Parameters**

- <N> : Bar number
- <addr> : Register address

::

	[xilinx@]# dmactl qdma17000 reg read bar 2 0x0
	qdma17000, 17:00.00, bar#2, 0x0 = 0x0.
	
Write a Register
----------------

command: ``dmactl qdma01000 reg write bar <N> <addr>``

Read a register value.

**Parameters**

- <N> : Bar number
- <addr> : Register address

::

	[xilinx@]# dmactl qdma17000 reg write bar 2 0x0 0
	qdma17000, 17:00.00, bar#2, reg 0x0 -> 0x0, read back 0x0.
	
Dump the Queue registers
------------------------

command: ``dmactl qdma01000 reg dump [dmap <Q> <N>]``

This command allows the user to dump the registers. Only dump dmap registers if dmap is specified. Specify dmap range to dump: Q=queue, N=num of queues.

**Parameters**

- <Q> : Queue number
- <N> : Number of queues

::

	[xilinx@]# dmactl qdma17000 reg dump dmap 1 2
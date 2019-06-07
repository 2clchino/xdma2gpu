QDMA Features
#############

QDMA Linux Driver supports the following list of features

QDMA Hardware Features
**********************

* SRIOV with 4 Physical Functions(PF) and 252 Virtual Functions(VF)
* Memory Mapped(MM) and Stream(ST) interfaces per queue
* 2048 queue sets

   * 2048 H2C (Host-to-Card) descriptor rings
   * 2048 C2H (Card-to-Host) descriptor rings
   * 2048 completion rings
* Supports Legacy and MSI-X Interrupts

   * 2048 MSI-X vectors.
   * Up to 8 MSI-X per function.
   * Interrupt Aggregation
   * User Interrupts
   * Error Interrupts
   * Legacy Interrupts : Supported only for PF0 with single queue   
* Mailbox communication between PF and VF driver
* Interrupt support for Mailbox events
* Flexible interrupt allocation between PF/VF
* HW Error reporting
* Zero byte transfers
* Immediate data transfers
* Descriptor bypass(8, 16, 32, 64 descriptor sizes) support
* Descriptor Prefetching
* Streaming C2H completion entry coalescing
* Disabling overflow check in completion ring
* Streaming H2C to C2H and C2H to H2C loopback support
* Dynamic queue configuration
* Completion ring descriptors of 8, 16, 32, 64 bytes sizes
* Flexible BAR mapping for QDMA configuration register space
* ECC support
* Completions in memory mapped mode
		

For details on Hardware Features refer to QDMA_Product_Guide_.

.. _QDMA_Product_Guide: https://www.xilinx.com/support/documentation/ip_documentation/qdma/v3_0/pg302-qdma.pdf

QDMA Software Features
**********************

* Polling and Interrupt Modes
	QDMA software provides 2 different drivers. PF driver for Physical functions and and VF driver for Virtual Functions.
	PF and VF drivers can be inserted in different modes.

   - Polling Mode 
		In Poll Mode, Software polls for the write back completions(Status Descriptor Write Back) 
		
   - Direct Interrupt Mode
		In Direct Interrupt mode, Each queue is assigned to one of the available interrupt vectors in a round robin fashion to service the requests. 
		Interrupt is raised by the HW upon receiving the completions and software reads the completion status.
		
   - Indirect Interrupt Mode
		In Indirect Interrupt mode or Interrupt Aggregation mode, each vector has an associated Interrupt Aggregation Ring. 
		The QID and status of queues requiring service are written into the Interrupt Aggregation Ring. 
		When a PCIe MSI-X interrupt is received by the Host, the software reads the Interrupt Aggregation Ring to determine which queue needs service. 
		Mapping of queues to vectors is programmable
		
   - Auto Mode
		Auto mode is mix of Poll and Interrupt Aggregation mode. Driver polls for the write back status updates.
		Interrupt aggregation is used for processing the completion ring.
		
- Allows only Privileged Physical Functions to program the contexts and registers
- Dynamic queue configuration
- Dynamic driver configuration
- Driver configuration through sysfs
- Asynchronous and Synchronous IO support
- Display the Version details for SW and HW
module hs_ad_da(
    input                 sys_clk     ,  //系统时钟
    input                 sys_rst_n   ,  //系统复位，低电平有效
	input                 uart_rxd    , 
	 //AD芯片接口
    input     [7:0]       ad_data     ,  //AD输入数据
    //DA芯片接口
    output                da_clk      ,  //DA(AD9708)驱动时钟,最大支持125Mhz时钟
    output    [7:0]       da_data     ,  //输出给DA的数据
	output				  ad_clk      ,
	output          	  uart_txd       //UART发送端口
);

//parameter define
parameter  CLK_FREQ = 50000000;       //定义系统时钟频率
parameter  UART_BPS = 115200;         //定义串口波特率
//wire define 
wire      [9:0]    rd_addr;           //ROM读地址
wire      [7:0]    rd_data;           //ROM_1M读出的数据

wire      [9:0]    rd_addr1;          //ROM读地址
wire      [7:0]    rd_data1;          //ROM_5M读出的数据

wire 	  [7:0]	   ad_data2;

wire      [10:0]   ram_addr;          //ROM读地址
wire      [7:0]    ram_data ;
wire			   ram_wr_en;
wire			   ram_rd_en;
wire      [10:0]   uart_addr;
wire      [10:0]   addr_ad;

wire		 	 		 loongson_en;
wire		 	 		 loongson_en2;
wire		 	 		 loongson_en3;
wire		 	 		 loongson_en1;
wire		[7:0] 	     uart_data_w;
wire					 clk_100M;
wire					 clk_30M;

assign ram_rd_en = loongson_en1;
assign ram_clk = ram_wr_en ? clk_30M : ram_rd_clk;
assign ram_addr = ram_wr_en ? addr_ad : uart_addr;

//*****************************************************
//**                    main code
//*****************************************************	
//接收255特殊信号
uart_recv #(                          //串口接收模块
    .CLK_FREQ       (CLK_FREQ),       //设置系统时钟频率
    .UART_BPS       (UART_BPS))       //设置串口接收波特率
u_uart_recv(                 
    .sys_clk        (sys_clk), 
    .sys_rst_n      (sys_rst_n), 
    .uart_rxd       (uart_rxd),
	 
    .uart_data      (uart_data_w)
    );
//使能信号
delay u_delay (
    .sys_clk        (sys_clk), 
	 .sys_rst_n      (sys_rst_n),
	 .uart_da      (uart_data_w),
	 
    .uart_en    	  (loongson_en),
	 .uart_en2    	  (loongson_en2),
	 .uart_en3    	  (loongson_en3),
    .uart_en1   	  (loongson_en1)	 
);
//ROM_1M存储波形
rom1	u_rom1 (
	.address ( rd_addr ),
	.clock ( sys_clk ),
	.q ( rd_data )
	);
	
//ROM_5M存储波形
rom2	u_rom2 (
	.address ( rd_addr1 ),
	.clock ( sys_clk ),
	.q ( rd_data1 )
	);	
	
//PLL	
pll u_pll (
    .inclk0        (sys_clk), 
	 .c0            (clk_100M),
	 .c1            (clk_30M)
);
//DA数据发送
da_wave_send u_da_wave_send(
    .sys_clk         (clk_100M), 
    .sys_rst_n       (loongson_en),
	.sys_rst_n1      (loongson_en3),
    .rd_data    	 	(rd_data),
	.rd_data1    	 	(rd_data1),
	 
	.rd_addr         (rd_addr),
	.rd_addr1        (rd_addr1),
    .da_clk     	   (da_clk),  
    .da_data     	   (da_data)
    );
//AD数据接收	
ad_wave_rec u_ad_wave_rec(                         
    .sys_rst_n       (loongson_en2),
	.clk_30M         (clk_30M),
    .ad_data     	 (ad_data),

	.addr_ad         (addr_ad),
	.ad_data_out	 (ad_data2),
    .ad_clk       	 (ad_clk),
	.ram_wr_en       (ram_wr_en)
    );
//RAM存储波形	 
ram1	u_ram1 (
	.clock( ram_clk ),
	.data ( ad_data2 ),
	.rden ( ram_rd_en ),
	.wren ( ram_wr_en ),
	.address ( ram_addr ),

	.q ( ram_data )
	);	
//读RAM发送串行数据
uart_send #(
    .CLK_FREQ       (CLK_FREQ),       //设置系统时钟频率
    .UART_BPS       (UART_BPS))       //设置串口接收波特率
u_uart_send(                 
    .sys_clk        (sys_clk),
    .sys_rst_n      (loongson_en1),  
	 
 	.ram_addr       (uart_addr),
    .uart_din       (ram_data ),
    .uart_txd       (uart_txd ),
	.ram_rd_clk     (ram_rd_clk)
    );
	 
endmodule	 

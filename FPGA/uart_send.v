module uart_send(
    input	      sys_clk,                  //系统时钟
    input         sys_rst_n,                //系统复位，低电平有效                
    input  [7:0]  uart_din,                 //待发送数据
	 
	output  reg  [8:0]    ram_addr,  		  //读RAM地址
    output  reg   uart_txd ,                //UART发送端口
	output  reg   ram_rd_clk
    );
    
//parameter define
parameter  CLK_FREQ = 50000000;             //系统时钟频率
parameter  UART_BPS = 115200;                 //串口波特率
localparam  BPS_CNT  = CLK_FREQ/UART_BPS;    //为得到指定波特率，对系统时钟计数BPS_CNT次

//reg define
reg        uart_en   ;

reg        uart_en_d0; 
reg        uart_en_d1;  
reg [15:0] clk_cnt;                         //系统时钟计数器
reg [ 3:0] tx_cnt;                          //发送数据计数器
reg        tx_flag;                         //发送过程标志信号
reg [ 7:0] tx_data;                         //寄存发送数据

//wire define
wire       en_flag;
//*****************************************************
//**                    main code
//*****************************************************
//parameter 
//频率调节控制
parameter  UART_ADJ = 14'd4340;  //UART传送一组数据用时4340*20ns= 86800ns
//reg define
reg    [15:0]    uart_cnt  ;  //UART传送一组数据计时

//频率调节计数器 
always @(posedge sys_clk or negedge sys_rst_n) begin
    if(sys_rst_n == 1'b0) begin
        uart_cnt <= 14'd0;
		  ram_rd_clk <= 1'd0;
		  uart_en <= 1'b0;
	end
	else begin
		  if(uart_cnt <= UART_ADJ/2) begin   
				ram_rd_clk <= 1'b1;
				uart_cnt <= uart_cnt + 1'b1;
				uart_en  <= 1'b0;
		  end
		  else if((uart_cnt > UART_ADJ/2)&&(uart_cnt < UART_ADJ)) begin
				ram_rd_clk <= 1'b0;
				uart_cnt <= uart_cnt + 8'b1;
				uart_en <= 1'b1;
		  end
		  else begin
				ram_rd_clk <= 1'b0;
				uart_cnt <= 8'b0;
				uart_en <= 1'b1;
			end
	 end
end

//RAM地址按照一个完整UART传送周期增加
always @(posedge ram_rd_clk or negedge sys_rst_n) begin
    if(sys_rst_n == 1'b0)
        ram_addr <= 9'd0;
	 else begin
		  if(ram_addr < 9'd511)
		  ram_addr <= ram_addr + 1'd1;		//+1速度太快  要让它在这期间只加这一次
	 else if(ram_addr == 9'd511)
		  ram_addr <= ram_addr;
	 end
end

//捕获uart_en上升沿，得到一个时钟周期的脉冲信号
assign en_flag = (~uart_en_d1) & uart_en_d0;

//对发送使能信号uart_en延迟两个时钟周期
always @(posedge sys_clk or negedge sys_rst_n) begin         
    if (!sys_rst_n) begin
        uart_en_d0 <= 1'b0;
        uart_en_d1 <= 1'b0;
    end
    else begin
        uart_en_d0 <= uart_en;                               
        uart_en_d1 <= uart_en_d0;                            
    end
end

//当脉冲信号uart_en到达时,寄存待发送的数据，并进入发送过程          
always @(posedge sys_clk or negedge sys_rst_n) begin         
    if (!sys_rst_n) begin                                  
        tx_flag <= 1'b0;
        tx_data <= 8'd0;
    end 
    else if (en_flag) begin            		//检测到发送使能上升沿                      
            tx_flag <= 1'b1;                //进入发送过程，标志位tx_flag拉高
            tx_data <= uart_din;            //寄存待发送的数据
        end
        else 
        if ((tx_cnt == 4'd9)&&(clk_cnt == BPS_CNT/2))
        begin                               //计数到停止位中间时，停止发送过程
            tx_flag <= 1'b0;                //发送过程结束，标志位tx_flag拉低
            tx_data <= 8'd0;
        end
        else begin
            tx_flag <= tx_flag;
            tx_data <= tx_data;
        end 
end

//进入发送过程后，启动系统时钟计数器与发送数据计数器
always @(posedge sys_clk or negedge sys_rst_n) begin         
    if (!sys_rst_n) begin                             
        clk_cnt <= 16'd0;                                  
        tx_cnt  <= 4'd0;
    end                                                      
    else if (tx_flag) begin                 //处于发送过程
        if (clk_cnt < BPS_CNT - 1) begin
            clk_cnt <= clk_cnt + 1'b1;
            tx_cnt  <= tx_cnt;
        end
        else begin
            clk_cnt <= 16'd0;               //对系统时钟计数达一个波特率周期后清零
            tx_cnt  <= tx_cnt + 1'b1;       //此时发送数据计数器加1
        end
    end
    else begin                              //发送过程结束
        clk_cnt <= 16'd0;
        tx_cnt  <= 4'd0;
    end
end

//根据发送数据计数器来给uart发送端口赋值
always @(posedge sys_clk or negedge sys_rst_n) begin        
    if (!sys_rst_n)  
        uart_txd <= 1'b1;        
    else if (tx_flag)
        case(tx_cnt)
				4'd0: uart_txd <= 1'b0;
            4'd1: uart_txd <= tx_data[0];   //数据位最低位
            4'd2: uart_txd <= tx_data[1];
            4'd3: uart_txd <= tx_data[2];
            4'd4: uart_txd <= tx_data[3];
            4'd5: uart_txd <= tx_data[4];
            4'd6: uart_txd <= tx_data[5];
            4'd7: uart_txd <= tx_data[6];
            4'd8: uart_txd <= tx_data[7];   //数据位最高位
				4'd9: uart_txd <= 1'b1;
            default: ;
        endcase
    else 
        uart_txd <= 1'b1;                   //空闲时发送端口为高电平
end

endmodule	

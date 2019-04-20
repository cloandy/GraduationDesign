--正在使用
module(...,package.seeall)

require"utils"
require"pm"

--串口ID,1对应uart1
local UART_ID = 1

--缓存数据
Global_Buff = ""
r1 = 0
r2 = 0
r3 = 0
--处理串口数据
local function proc(data)
    
	Global_Buff = data
	log.info("Global_Buff",Global_Buff)
	r1,r2,r3 = string.byte(Global_Buff,1,3)
	log.info("Proc.read bin",r1,r2,r3)
	
	r1 = r1 - 48;
	r3 = r3 - 48;
	log.info("Proc.read DEC",r1,r3)
	
end

--接收串口数据
local function read()
    local data = ""
    --底层core中，串口收到数据时：
    --如果接收缓冲区为空，则会以中断方式通知Lua脚本收到了新数据；
    --如果接收缓冲器不为空，则不会通知Lua脚本
    --所以Lua脚本中收到中断读串口数据时，每次都要把接收缓冲区中的数据全部读出，这样才能保证底层core中的新数据中断上来，此read函数中的while语句中就保证了这一点
    while true do
        data = uart.read(UART_ID,"*l")
        --数据不存在时停止接收数据
        if not data or string.len(data) == 0 then 
			break 
		end
        --打开下面的打印会耗时
--        log.info("testUart.read bin",data)
--        log.info("testUart.read hex",data:toHex())
		
        --真正的串口数据处理函数
        proc(data)
    end
end

--发送串口数据
function write(s)
    log.info("testuart.write",s:toHex(),s)
    uart.write(UART_ID,s)
end

--保持系统处于唤醒状态，此处只是为了测试需要，所以此模块没有地方调用pm.sleep("testUart")休眠，不会进入低功耗休眠状态
--在开发“要求功耗低”的项目时，一定要想办法保证pm.wake("testUart")后，在不需要串口时调用pm.sleep("testUart")

pm.wake("testUart")
--注册串口的数据接收函数：
--						串口收到数据后，会以中断方式，调用read接口读取数据
uart.on(UART_ID,"receive",read)

--配置并且打开串口:
--				串口1
--				波特率  ：115200
--				数据位数：8位
--				奇偶校验：无校验
--				停止位  ：1
uart.setup(UART_ID,115200,8,uart.PAR_NONE,uart.STOP_1)















--[[
module(...,package.seeall)
require"utils"
require"pm"
--串口ID,1对应uart1
--如果要修改为uart2，把UART_ID赋值为2即可
local UART_ID = 1
--缓存数据
local buf = ""
--处理串口数据
local function proc(data)
    data = buf..data
    log.info("testUart.read proc",data)
    local used = true--数据是否被处理？
    if data == "qwerty" then
        --模块收到`qwerty`字符串后，回复`asdfgh`字符串
        write("asdfgh")
    elseif data == string.fromHex("AABBCC") then
        --模块收到`0xaa 0xbb 0xcc`三个字节后，回复`0xdd 0xee 0xff`三个字节
        write(string.fromHex("DDEEFF"))
    elseif data == "abcdefghijklmnopqrstuvwxyz" then
        --模块收到`abcdefghijklmnopqrstuvwxyz`字符串后，回复`ok`字符串
        write("ok")
    else
        --数据没匹配上任何东西，没被使用
        used = false
    end
    if not used then--数据没被使用
        if buf == "" then--如果缓冲区是空的
            sys.timerStart(function()
                buf = ""
            end,500)--500ms后清空缓冲区
        end
        buf = data--数据追加到缓存区
    else
        buf = ""
    end
end
--接收串口数据
local function read()
    local data = ""
    --底层core中，串口收到数据时：
    --如果接收缓冲区为空，则会以中断方式通知Lua脚本收到了新数据；
    --如果接收缓冲器不为空，则不会通知Lua脚本
    --所以Lua脚本中收到中断读串口数据时，每次都要把接收缓冲区中的数据全部读出，这样才能保证底层core中的新数据中断上来，此read函数中的while语句中就保证了这一点
    while true do
        data = uart.read(UART_ID,"*l")
        --数据不存在时停止接收数据
        if not data or string.len(data) == 0 then break end
        --打开下面的打印会耗时
        log.info("testUart.read bin",data)
        log.info("testUart.read hex",data:toHex())
        --真正的串口数据处理函数
        proc(data)
    end
end
--发送串口数据
function write(s)
    log.info("testuart.write",s:toHex(),s)
    uart.write(UART_ID,s)
end
--保持系统处于唤醒状态，此处只是为了测试需要，所以此模块没有地方调用pm.sleep("testUart")休眠，不会进入低功耗休眠状态
--在开发“要求功耗低”的项目时，一定要想办法保证pm.wake("testUart")后，在不需要串口时调用pm.sleep("testUart")
pm.wake("testUart")
--注册串口的数据接收函数，串口收到数据后，会以中断方式，调用read接口读取数据
uart.on(UART_ID,"receive",read)
--配置并且打开串口
uart.setup(UART_ID,9600,8,uart.PAR_NONE,uart.STOP_1)
--模块开机第10秒后，向设备发送`0x01 0x02 0x03`三个字节
sys.timerStart(function()
    write(string.fromHex("010203"))
end,10000)

--]]










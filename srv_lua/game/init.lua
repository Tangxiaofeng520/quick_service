
package.path = '/home/diandian/Desktop/quick_service/srv_lua/?.lua'
local srv_mgr = require "srv_mgr"
local srv_typedef = require "shared_table.srv_typedef"
function OnInit(id)
    srv_mgr.add_srv(srv_typedef.game,id)
    print("lua game OnInit "..id)
end


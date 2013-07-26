sina_appid = "2376942997"
sina_secret = "33a1e119d4ffe60359c24ad97c17363e"
sina_cb_http = "http://127.0.0.1:8000/"
hostname = "127.0.0.1"

msg_text = "茄子"
has_auth = false

sina_auth = ->
    url1 = "https://api.weibo.com/oauth2/authorize?client_id="
    url2 = "&response_type=code&redirect_uri="
    auth_url = url1 + sina_appid + url2 + sina_cb_http
    
    location.href= auth_url

sina_get_code = ->
    code1 = location.search
    echo code1

    strs = code1.split("=")
    echo strs
    code_len = strs[1].length
    sina_code = strs[1].slice(0, code_len - 1)
    return sina_code

sina_get_token = ->
    code = sina_get_code()
    get_token_xhr = new XMLHttpRequest()
    get_token_url = "https://api.weibo.com/oauth2/access_token?client_id="+sina_appid+"&client_secret="+sina_secret+"&grant_type=authorization_code&code="+code+"&redirect_uri="+sina_cb_http
    
    echo get_token_url
    get_token_xhr.open("POST", get_token_url, false)
    get_token_xhr.send(null)
    response = get_token_xhr.responseText
    get_token_xhr = null
    has_auth = true
    echo response

    echo response.length
    if response.length == 117
        json_obj = JSON.parse(response)
        #echo response.slice(17, 49)
        echo "access_token: " + json_obj.access_token
        sina_access_token = json_obj.access_token
        echo sina_get_username(sina_access_token)
        DCore.WeiboTest.SaveToken(sina_access_token)
        DCore.WeiboTest.SinaUpload("Test XMLHttpRequest")
        has_exec = true

sina_parse_onload = ->
    hostdest = location.hostname

    echo hostdest
    if hostdest in [hostname]
        sina_get_token()

sina_get_username = (access_token)->
    id_xhr = new XMLHttpRequest()
    id_url = "https://api.weibo.com/2/account/get_uid.json?&access_token="+access_token
    echo "id: "+id_url

    id_xhr.open("GET", id_url, false)
    id_xhr.send(null)
    response = id_xhr.responseText
    json_obj = JSON.parse(response)
    sina_uid = json_obj.uid
    echo sina_uid
    id_xhr = null

    id_xhr = new XMLHttpRequest()
    id_url = "https://api.weibo.com/2/users/show.json?&access_token="+access_token+"&uid="+sina_uid
    echo "show: "+id_url

    id_xhr.open("GET", id_url, false)
    id_xhr.send(null)
    response = id_xhr.responseText
    id_xhr = null
    json_obj = JSON.parse(response)
    sina_username = json_obj.screen_name
    return sina_username

class WeiboTest extends Widget
    constructor: (@id)->
        super()
        echo "3333"
        @element.innerText = "X"
        $("#close").appendChild(@element)

    do_click: (e)->
        echo "exit weibo..."
        DCore.WeiboTest.exit()

class WeiboShare extends Widget
    constructor: (@id)->
        super
        echo "567898"
        @element.innerText = "Share"
        $("#button").appendChild(@element)
        
    do_click: (e)->
        if has_auth == true
            DCore.WeiboTest.SinaUpload(msg_text)
            return
        echo "sina authorize..."
        sina_auth()

new WeiboTest("_add_close_button")
new WeiboShare("_add_share_button")
window.addEventListener('load', sina_parse_onload, false)

has_auth = false
textArea = document.getElementById("text_id")

checkLength = (which)->
    which = event.target
    maxLength = 140
    chNum = document.getElementById("chLeft")
    curLength = maxLength - which.value.length

    if curLength < 0
        chNum.style.color = "red"
    
    chNum.innerHTML = curLength.toString()

class SinaWeibo
    constructor: ->
        @sinaAppid = "2376942997"
        @sinaSecret = "33a1e119d4ffe60359c24ad97c17363e"
        @sinaCallBackHttp = "http://127.0.0.1:8000/"
        @hostName = "127.0.0.1"

    SinaAuth: ->
        url1 = "https://api.weibo.com/oauth2/authorize?client_id="
        url2 = "&response_type=code&redirect_uri="
        url = url1 + @sinaAppid + url2 + @sinaCallBackHttp
        echo "Auth url: " + url

        location.href= url

    SinaGetCode: ->
        code1 = location.search
        strs = code1.split("=")
        code_len = strs[1].length
        code = strs[1].slice(0, code_len - 1)
        echo "Code: " + code

        return code

    SinaGetToken: ->
        url1 = "https://api.weibo.com/oauth2/access_token?client_id="
        url2 = "&client_secret="
        url3 = "&grant_type=authorization_code&code="
        url4 = "&redirect_uri="
        code = @SinaGetCode()
        url = url1 + @sinaAppid + url2 + @sinaSecret + url3 + code + url4 + @sinaCallBackHttp

        echo "Token url: " + url
        xhr = new XMLHttpRequest()
        xhr.open("POST", url, false)
        xhr.send(null)
        response = xhr.responseText
        xhr = null

        echo response
        if response.length == 117
            has_auth = true
            jsonObj = JSON.parse(response)
            @sinaAccessToken = jsonObj.access_token
            
            echo "Token: " + @sinaAccessToken
            DCore.WeiboTest.SaveToken(@sinaAccessToken)
            @SinaGetUid()
            DCore.WeiboTest.SinaUpload()
            return @sinaAccessToken
        else
            return null

    SinaGetUid: ()->
        url1 = "https://api.weibo.com/2/account/get_uid.json?&access_token="
        url = url1 + @sinaAccessToken
        xhr = new XMLHttpRequest()

        echo "Uid url: " + url
        xhr.open("GET", url, false)
        xhr.send(null)
        response = xhr.responseText
        xhr = null

        jsonObj = JSON.parse(response)
        sinaUid = jsonObj.uid
        echo "Uid: " + sinaUid
        return sinaUid

    SinaGetUserName: (@token, @uid)->
        xhr = new XMLHttpRequest()
        url1 = "https://api.weibo.com/2/users/show.json?&access_token="
        url = url1 + @token + "&uid=" + uid

        echo "UserName url: " + url
        xhr.open("GET", url, false)
        xhr.send(null)
        response = xhr.responseText
        xhr = null

        jsonObj = JSON.parse(response)
        sinaUserName = jsonObj.screen_name
        echo "UserName: " + sinaUserName
        return sinaUserName

    SinaParseLoad: =>
        hostdest = location.hostname

        echo "dest: " + hostdest
        search = location.search
        echo "search: " + search
        if search == null
            return
        if hostdest == @hostName
            @SinaGetToken()

sinaHandle = new SinaWeibo()

###
class WeiboClose extends Widget
    constructor: (@id)->
        super()
        @img = create_img('', 'img/cancel_normal.png', @element)
        $("#title-pos").appendChild(@element)

    do_click: (e)=>
        DCore.WeiboTest.exit()

    do_mousedown: (e)=>
        @img.src = "img/cancel_press.png"

    do_mouseover: (e)=>
        @img.src = 'img/cancel_hover.png'

    do_mouseout: (e)=>
        @img.src = "img/cancel_normal.png"
###

class WeiboShare extends Widget
    #@sinaText: null
    constructor: (@id)->
        super()
        #@input = document.createElement('input')
        #@input.type = 'button'
        #@input.value = "Share"
        #$("#button-pos").appendChild(@input)
        @element.innerText = "Share"
        $("#button-pos").appendChild(@element)

    do_click: (e)=>
        echo "Share button clicked"
        echo "click textArea: " + textArea.value
        sinaText = textArea.value
        DCore.WeiboTest.SaveMsg(sinaText)
        if has_auth
            DCore.WeiboTest.SinaUpload()
            return
        sinaHandle.SinaAuth()

#new WeiboClose("_add_close")
#new WeiboShare("_add_share")
window.addEventListener('load', sinaHandle.SinaParseLoad, false)
textArea.addEventListener('input', checkLength)

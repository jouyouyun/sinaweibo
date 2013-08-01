has_auth = false
textArea = document.getElementById("text_id")
closeButton = document.getElementById("close_id")
sinaPicAuth = document.getElementById("sina_pic_id")
shareButton = document.getElementById("share_id")
sinaCheckBox = document.getElementById("sinaCheckId")
bodyContainer = document.getElementById("containerId")

checkLength = (which)->
    which = event.target
    maxLength = 140
    chNum = document.getElementById("chLeft")
    curLength = maxLength - which.value.length

    if curLength < 0
        chNum.style.color = "red"
    else
        chNum.style.color = "#94a9c8"
    
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
            sinaPicAuth.src = "../img/Sina.png"

            jsonObj = JSON.parse(response)
            @sinaAccessToken = jsonObj.access_token
            
            echo "Token: " + @sinaAccessToken
            DCore.WeiboTest.SaveToken(@sinaAccessToken)
            @sinaUserName = @SinaGetUserName(@sinaAccessToken, @SinaGetUid())
            #new SinaUserLabel()
            sinaPicAuth.value = @sinaUserName
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

WeiboExit = =>
    DCore.WeiboTest.exit()

WeiboLogin = =>
    if !has_auth
        sinaHandle.SinaAuth()
        #sinaHandle.SinaGetToken()

WeiboPublish = =>
    if has_auth
        if !sinaCheckBox.checked
            alert("请选择需要发送的微博")
            return
        else if textArea.value.length < 1
            alert("请输入发送内容")
            return
        else
            DCore.WeiboTest.SaveMsg(textArea.value)
            DCore.WeiboTest.SinaUpload()
            textArea.value = ""
            return

class SinaUserLabel extends Widget
    constructor: (@id)->
        super()
        echo "UserName: " + sinaHandle.sinaUserName
        @element.innerText = sinaHandle.sinaUserName
        $("#sina_button").appendChild(@element)

SinaUploadOver = =>
    alert("Weibo Send Over")
    DCore.WeiboTest.exit()

window.addEventListener('load', sinaHandle.SinaParseLoad, false)
textArea.addEventListener('input', checkLength)
sinaPicAuth.addEventListener('click', WeiboLogin)
closeButton.addEventListener('click', WeiboExit)
shareButton.addEventListener('click', WeiboPublish)

DCore.signal_connect("SinaUploadComplete", ->
    SinaUploadOver())
host = "127.0.0.1"
dest = "127.0.0.1"
array = [host, '127']

console.log(array)
###
if(host === dest){
    console.log("dest == host")
}

if(dest in [host]) {
    console.log("dest in host")
}
###

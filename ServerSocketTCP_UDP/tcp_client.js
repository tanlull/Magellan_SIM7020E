const net = require('net')
const conf = require('./config/config')

const client = new net.Socket()
const host = conf.serverHost

client.connect(conf.port, host, function() {
    console.log('Connected')
    client.write("Hello From Client " + client.address().address)
})

client.on('data', data => {
    console.log('Received: ' + data)
    client.destroy()
})

client.on('close', () => {
    console.log('Connection closed')
})
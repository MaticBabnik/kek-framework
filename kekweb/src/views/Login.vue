<script lang="ts" setup >
import { ref } from 'vue';
import { useRouter } from 'vue-router';

const username = ref<string>("");
const password = ref<string>("");

const r = useRouter();
function login() {
    localStorage.setItem("username",username.value);
    localStorage.setItem("password",password.value);

    //do stuff
    r.replace('/');
}
</script>

<template>
    <div id="login">
        <h1>Login</h1>
        <input type="text" autocomplete="username" name="Username" id="username" placeholder="Username"
            @keypress.enter="login" v-model="username">
        <input type="password" autocomplete="current-password" name="Password" id="password" placeholder="Password"
            @keypress.enter="login" v-model="password">
        <input type="button" id="go" value="Go" @keypress.enter="login" @click="login">
    </div>
</template>

<style lang="less" scoped>
h1 {
    font-size: 3rem;
    margin: 1rem;
}

#login {
    display: flex;
    flex-direction: column;
}

#username,
#password,
#go {
    padding: .5rem;
    margin: .25rem;
    outline: none;
    border: none;
    color: #fff;
    background-color: #0005;
    border-radius: 1rem;

    &:active,
    &:focus {
        transition: .2s;
        box-shadow: 0 0 .2rem #0cf;
        background-color: #0006;
    }
}
</style>
<script lang="ts" setup >
import { ref } from 'vue';
import { useRouter } from 'vue-router';
import { useAuthStore } from '../stores/authStore';

const auth = useAuthStore();
const r = useRouter();

const username = ref<string>("");
const password = ref<string>("");

async function login() {
    try {
        await auth.login(username.value, password.value);
        localStorage.setItem("username", username.value);
        localStorage.setItem("password", password.value);

        r.replace('/');
    } catch {
        alert("Invalid username/password");
        username.value = "";
        password.value = "";
    }
}
</script>

<template>
    <div class="center-login">
        <div id="login">
            <h1>Login</h1>
            <input class="button" type="text" autocomplete="username" name="Username" id="username"
                placeholder="Username" @keypress.enter="login" v-model="username">
            <input class="button" type="password" autocomplete="current-password" name="Password" id="password"
                placeholder="Password" @keypress.enter="login" v-model="password">
            <input class="button" type="button" id="go" value="Go" @keypress.enter="login" @click="login">
        </div>
    </div>
</template>

<style lang="less" scoped>
#login h1 {
    font-size: 3rem;
    margin: 1rem;
    text-align: center;
}

#login {
    display: flex;
    flex-direction: column;
}

#username,
#password,
#go {
    margin: .25rem;
}
</style>

<style lang="less">
.center-login {
    display: grid;
    place-items: center;
}
</style>
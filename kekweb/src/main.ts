import { createApp } from 'vue'
import './style.less'
import App from './App.vue'
import { createRouter, createWebHashHistory } from 'vue-router'
import { createPinia } from "pinia"
import { useAuthStore } from "./stores/authStore";

import modules from "./components/modules"

import Home from "./views/Home.vue";
import Login from "./views/Login.vue";

const router = createRouter({
    history: createWebHashHistory("/"),
    routes: [
        {
            path: "/",
            meta: {
                auth: true
            },
            component: Home
        },
        {
            path: "/login",
            component: Login
        },
    ]
});

const pinia = createPinia();

const app = createApp(App)
    .use(router)
    .use(pinia);

Object.entries(modules).forEach(([name, module]) => {
    app.component(name, module);
});

app.mount('#app');


(async () => {
    const auth = useAuthStore();

    const username = localStorage.getItem('username')
    const password = localStorage.getItem('password')

    if (username && password) {
        try {
            await auth.login(username, password);
            router.push("/");
        } catch {
            localStorage.removeItem('username');
            localStorage.removeItem('password');
            router.push("/login");
        }
    }

    router.beforeEach((to, from, next) => {
        const authStore = useAuthStore();
        if (to.meta.auth && !authStore.valid) {
            next("/login");
        } else {
            next();
        }
    });
})();
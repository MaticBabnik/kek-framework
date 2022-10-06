import { defineStore } from "pinia";


export const useAuthStore = defineStore("auth", {
    state() {
        return {
            username: "",
            password: "",
            valid: false,
        }
    },
    getters: {
        credentials(state) {
            if (!this.valid) {
                throw "Not authenticated";
            }
            return {
                username: state.username,
                password: state.password,
            };
        },
        header(state) {
            if (!this.valid) {
                throw "Not authenticated";
            }
            return {
                "Authorization": `Basic ${btoa(`${state.username}:${state.password}`)}`
            };
        }
    },
    actions: {
        async login(username: string, password: string) {
            const response = await fetch(`${import.meta.env.VITE_API_URL}/api/auth`, {
                method: "GET",
                headers: {
                    "Authorization": `Basic ${btoa(`${username}:${password}`)}`
                }
            });

            if (response.status === 200) {
                this.$state.username = username;
                this.password = password;
                this.valid = true;
            } else {
                throw "Invalid credentials";
            }

            return true;
        },
        logout() {
            this.username = "";
            this.password = "";
            this.valid = false;
            localStorage.removeItem('username');
            localStorage.removeItem('password');
        }
    }
});
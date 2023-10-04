import { defineStore } from "pinia";
import { useAuthStore } from "./authStore";

export const useKekStore = defineStore("kek", {
    state() {
        return {
            maxReadInterval: 1000,
            lastRead: 0,
            initalized: false,
            data: {} as Record<string, string | Object>,
            modules: {} as Record<string, string>,
        };
    },
    actions: {
        async init(maxReadInterval?: number) {
            if (this.initalized) return this.modules;

            if (typeof maxReadInterval === "number")
                this.maxReadInterval = maxReadInterval;

            const auth = useAuthStore();

            console.log(auth.header);

            const modulesR = await fetch(
                `${import.meta.env.VITE_API_URL}/api/list`,
                {
                    method: "GET",
                    headers: auth.header,
                }
            );

            const dataR = await fetch(
                `${import.meta.env.VITE_API_URL}/api/read`,
                {
                    method: "POST",
                    headers: auth.header,
                }
            );

            this.modules = await modulesR.json();
            this.data = await dataR.json();

            this.initalized = true;
        },
        /**
         * Reads data from the server or returns cached data
         */
        async readAll() {
            if (this.lastRead + this.maxReadInterval > Date.now())
                return this.data;

            this.lastRead = Date.now();

            const auth = useAuthStore();

            const dataR = await fetch(
                `${import.meta.env.VITE_API_URL}/api/read`,
                {
                    method: "POST",
                    headers: auth.header,
                }
            );

            this.data = await dataR.json();
            return this.data;
        },
        /**
         * The main write method; can write to multiple modules at once
         */
        async writeMany(
            data: Record<string, any>
        ): Promise<Record<string, any>> {
            const auth = useAuthStore();
            const writeR = await fetch(
                `${import.meta.env.VITE_API_URL}/api/write`,
                {
                    method: "POST",
                    headers: {
                        ...auth.header,
                        Accept: "application/json",
                        "Content-Type": "application/json",
                    },
                    body: JSON.stringify(data),
                }
            );

            const response = await writeR.json();
            return response;
        },
        /**
         * A wrapper around `writeMany` that writes to a single module
         */
        async write(modId: string, data: any) {
            return (await this.writeMany({ [modId]: data }))[modId];
        },
        /**
         * A wrapper around `write` that writes to a single module and updates the store's data
         */
        async writeAndUpdate(modId: string, data: any) {
            const response = await this.write(modId, data);
            this.data[modId] = data;
            return response;
        },
        /**
         * A wrapper around `write` that writes to a single module and updates the store's data
         */
        async writeAndUpdateSane(modId: string, data: any) {
            const response = await this.write(modId, data);
            return (this.data[modId] = response);
        },
        /**
         * A wrapper around `writeMany` that writes to multiple modules and updates the store's data
         */
        async writeManyAndUpdate(
            data: Record<string, any>
        ): Promise<Record<string, any>> {
            const response = await this.writeMany(data);
            this.data = { ...response, ...this.data };
            return response;
        },
        /**
         * Returns the module data from the store's current state
         */
        fromCache(id: string): string | Object | undefined {
            return this.data[id];
        },
    },
});

# Lightdome Frontend

This folder contains the (dockerized) development stuff for the web frontend.

- Initally build the image
```
docker-compose -f docker-compose-frontend.yml build --no-cache
```

- Run the container in idle mode (no build process will be startet)
```
docker-compose -f docker-compose-frontend.yml up -d
```

- Start the development (server)
```
docker exec -it lightdome_frontend /bin/sh 
```

- Build
```
docker exec -it lightdome_frontend /bin/sh -c "npm build"
```
> a folder will be created in: `frontend/build`


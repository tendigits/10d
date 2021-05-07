# Redirects

I'm keeping a list of the redirects used in the nginx server config, in case I need to rebuild the server. This list doesn't do anything directly, it's just a reference.

    rewrite ^/about/$ /site/about.html permanent;
    rewrite ^/about/nate-steiner/$ /site/nate-steiner.html permanent;
    rewrite ^/about/now$ /site/about.html#now permanent;
    rewrite ^/about/using$ /site/about.html#using permanent;
    rewrite ^/notes/ae-modular/$ /site/ae_modular.html permanent;
    rewrite ^/notes/bagels/$ /site/bagels.html permanent;
    rewrite ^/notes/hello/$ /site/website.html permanent;
    rewrite ^/notes/house/$ /site/house.html permanent;
    rewrite ^/notes/imac-with-linux/$ /site/imac_linux.html permanent;
    rewrite ^/notes/inktober/$ /site/inktober.html permanent;
    rewrite ^/notes/new-projects/$ /site/log.html permanent;
    rewrite ^/notes/post-receive-hook/$ /site/postreceivehook.html permanent;
    rewrite ^/notes/site-11ty-setup/$ /site/website.html permanent;
    rewrite ^/notes/updating-ssh/$ /site/ssl.html permanent;
    rewrite ^/projects/bird-posters/$ /site/bird_posters.html permanent;
    rewrite ^/projects/birdhouse-cam/$ /site/birdhouse_cam.html permanent;
    rewrite ^/projects/orca-box/$ /site/orca_box.html permanent;
program caf_hello

  character*80 hostname
  call hostnm(hostname)
  write(*,*) "Hello from image ", this_image(), &
             "running on ", trim(hostname), &
             " out of ", num_images()

end program caf_hello

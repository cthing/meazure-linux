window.addEventListener('DOMContentLoaded', () => {
    const observer = new IntersectionObserver(entries => {
        entries.forEach(entry => {
            const id = entry.target.getAttribute('id');
            const elem = document.querySelector(`nav li a[href="#${id}"]`);
            if (elem !== null) {
                const classes = elem.parentElement.classList;
                if (entry.intersectionRatio > 0) {
                    classes.add('active');
                } else {
                    classes.remove('active');
                }
            }
        });
    });

    // Track all sections that have an `id` applied
    document.querySelectorAll('section[id]').forEach((section) => {
        observer.observe(section);
    });
});

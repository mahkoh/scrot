pkgname=scrot-git
pkgver=0.9.1
pkgrel=1
pkgdesc="A simple command-line screenshot utility for X"
arch=('i686' 'x86_64')
url="https://github.com/mahkoh/scrot"
license=('MIT')
depends=('libx11' 'imlib2')
conflicts=('scrot')
provides=('scrot')
source=("$pkgname"::'git+https://github.com/mahkoh/scrot.git')
md5sums=('SKIP')

build() {
  cd $pkgname
  make
}

package() {
  cd $pkgname
  make PREFIX=/usr DESTDIR=$pkgdir install
}
